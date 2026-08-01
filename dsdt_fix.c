
#include "uefi.h"

#pragma push_macro("_MSC_VER")
#undef _MSC_VER
#define UACPI_OVERRIDE_TYPES
#include "uacpi/uacpi.h"
#include "uacpi/resources.h"
#include "uacpi/utilities.h"
#pragma pop_macro("_MSC_VER")

#include "definitions.h"
#include "hob.h"
#include "aml.h"
#include "dsdt_fix.h"

typedef struct _devpath_list {
    struct _devpath_list *next;
    size_t name_size;
    char *name;
} devpath_list;

typedef struct _dsdt_walker_data {
    devpath_list *offline_ddrcs; // out: offline DDRCs
    devpath_list *missing_hids; // out: missing HID devices
    uintptr_t online_ddrcs_base[MAX_SOCKET * MAX_CHANNEL]; // in: online DDRCs base addresses
} dsdt_walker_data;

static inline void devpath_list_append_uacpi_object_names(
    devpath_list **list, const uacpi_object_name *names, uint8_t depth
) {
    size_t size;
    char *name;

    // generate names and size
    if (depth == 2) {
        // use RootChar '\\', DualNamePrefix '.', parts to encode it
        size = 10;
        name = (char *)malloc(size);
        if (!name)
            return;
        name[0] = '\\';
        name[1] = '.';
        memcpy(name + 2, names, 8);
    } else {
        // use RootChar '\\', MultiNamePrefix '/', num of parts, parts to encode it
        size = 3 + (depth) * 4;
        name = (char *)malloc(size);
        if (!name)
            return;
        name[0] = '\\';
        name[1] = '/';
        name[2] = (char)depth;
        memcpy(name + 3, names, 4 * depth);
    }

    // append to list
    devpath_list **p_new_node = list;
    // find the end of the list
    while (*p_new_node) {
        p_new_node = &(*p_new_node)->next;
    }
    *p_new_node = (devpath_list *)malloc(sizeof(devpath_list));
    if (!*p_new_node) {
        return;
    }
    (*p_new_node)->next = NULL;
    (*p_new_node)->name_size = size;
    (*p_new_node)->name = name;
}

const efi_guid_t acpi_guid = ACPI_TABLE_GUID;
const efi_guid_t acpi2_guid = ACPI_20_TABLE_GUID;

static uacpi_iteration_decision crs_callback(
    void *user,
    uacpi_resource *res
) {
    uintptr_t *base = (uintptr_t *)user;

    switch (res->type) {
    case UACPI_RESOURCE_TYPE_ADDRESS64:
        *base = (uintptr_t)res->address64.minimum;
        return UACPI_ITERATION_DECISION_BREAK;
    }
    return UACPI_ITERATION_DECISION_CONTINUE;
}

static uacpi_iteration_decision dsdt_walker(
    void *user, uacpi_namespace_node *node, uacpi_u32 node_depth
) {
    dsdt_walker_data *data = (dsdt_walker_data *)user;

    uacpi_iteration_decision ret = UACPI_ITERATION_DECISION_CONTINUE;
    uacpi_namespace_node_info *info;
    uacpi_status status = uacpi_get_namespace_node_info(node, &info);
    uacpi_size depth = uacpi_namespace_node_depth(node);

    uacpi_object_name names[256];

    // skip bad node
    if (status != UACPI_STATUS_OK) {
        goto out;
    }
    if (depth < 2 || depth > 255) {
        // ??? '\\' cannot be a device and cannot more than 255 levels deep
        // ignore it
        goto out;
    }

    // prepare names
    {
        uacpi_namespace_node *cur = node;
        for (uacpi_size i = depth; i > 0; ) {
            i--;
            names[i] = uacpi_namespace_node_name(cur);
            cur = uacpi_namespace_node_parent(cur);
        }
    }

    // for Windows, devices must have _HID and _ADR methods
    // so we record it at there:
    if ((info->flags & (UACPI_NS_NODE_INFO_HAS_HID | UACPI_NS_NODE_INFO_HAS_ADR)) == 0) {
        // missing _HID or _ADR, record it
        devpath_list_append_uacpi_object_names(&data->missing_hids, names, (uint8_t)depth);
        const char *fullname = uacpi_namespace_node_generate_absolute_path(node);
        printf("missing _HID or _ADR: %s\n", fullname);
        uacpi_free_absolute_path(fullname);
    }

    // for DDRC devices, we check if it's _CRS in online range, if not, we record it as offline
    if (info->flags & UACPI_NS_NODE_INFO_HAS_HID) {
        uacpi_u32 sta;
        if (uacpi_eval_sta(node, &sta) == UACPI_STATUS_OK && sta == 0) {
            // already disabled, skip it
            goto out;
        }

        if (!strcmp(info->hid.value, "HISI0233")) {
            // check _CRS base
            uintptr_t addr;
            status = uacpi_for_each_device_resource(
                node,
                "_CRS",
                crs_callback,
                &addr
            );
            if (status != UACPI_STATUS_OK) {
                goto out;
            }

            for (size_t i = 0; i < MAX_SOCKET * MAX_CHANNEL; i++) {
                if (data->online_ddrcs_base[i] != 0 && data->online_ddrcs_base[i] == addr) {
                    // online, skip it
                    goto out;
                }
            }

            // offline, record it
            devpath_list_append_uacpi_object_names(&data->offline_ddrcs, names, (uint8_t)depth);
            const char *fullname = uacpi_namespace_node_generate_absolute_path(node);
            printf("offline DDRC: %s at 0x%016x\n", fullname, addr);
            uacpi_free_absolute_path(fullname);
        }
    }

out:
    if (info) {
        uacpi_free_namespace_node_info(info);
    }
    return ret;
}

extern uint64_t midr; // at tablesfix.c

int fix_dsdt(EFI_ACPI_TABLE_PROTOCOL *acpi_table) {
    dsdt_walker_data data = { NULL, NULL, { 0 } };

    if ((midr & 0xFFFFFFFF) == 0x481fd010) {
        // Hisilicon Hi1620
        // if this failed, we assume all DDRCs are offline
        (void)hob_get_online_ddrcs(acpi_table, data.online_ddrcs_base);
    }

    {
        uacpi_status ret = uacpi_initialize(0);
        if (UACPI_STATUS_OK != ret) {
            printf("uacpi_initialize(0) failed: %s\n", uacpi_status_to_string(ret));
            return 1;
        }

        ret = uacpi_namespace_load();
        if (UACPI_STATUS_OK != ret) {
            printf("uacpi_namespace_load() failed: %s\n", uacpi_status_to_string(ret));
            uacpi_state_reset();
            return 1;
        }

        uacpi_namespace_for_each_child(
            uacpi_namespace_root(),
            dsdt_walker,
            NULL,
            UACPI_OBJECT_DEVICE_BIT,
            UACPI_MAX_DEPTH_ANY,
            &data
        );

        uacpi_state_reset();
    }

    if (data.missing_hids != NULL || data.offline_ddrcs != NULL) {
        // build ssdt
        uint16_t missing_hids_count = 0, offline_ddrcs_count = 0;
        for (devpath_list *node = data.missing_hids; node != NULL; node = node->next) {
            missing_hids_count++;
        }
        for (devpath_list *node = data.offline_ddrcs; node != NULL; node = node->next) {
            offline_ddrcs_count++;
        }
        aml_node *root = aml_alloc_node(AML_OPCODE__ROOT, 1 + missing_hids_count + offline_ddrcs_count);
        uint16_t root_index = 0;
        {
            // externals
            uint16_t if_zero_index = 0;
            aml_node *if_zero = aml_alloc_node(AML_OPCODE_IfOp, 1 + missing_hids_count + offline_ddrcs_count);
            aml_node *zero = aml_alloc_node(AML_OPCODE_ZeroOp, 0);
            if_zero->children[if_zero_index++] = zero;

            for (devpath_list *node = data.missing_hids; node != NULL; node = node->next) {
                // build ExternalObj(name, DeviceObj)
                aml_node *external = aml_alloc_node(AML_OPCODE_ExternalOp, 3);
                external->children[0] = aml_data_node(node->name, (uint16_t)node->name_size);
                external->children[1] = aml_data_node("\x06", 1); // DeviceObj
                external->children[2] = aml_data_node("\x00", 1); // ArgumentCount = 0

                if_zero->children[if_zero_index++] = external;
            }

            for (devpath_list *node = data.offline_ddrcs; node != NULL; node = node->next) {
                // build ExternalObj(name, DeviceObj)
                aml_node *external = aml_alloc_node(AML_OPCODE_ExternalOp, 3);
                external->children[0] = aml_data_node(node->name, (uint16_t)node->name_size);
                external->children[1] = aml_data_node("\x06", 1); // DeviceObj
                external->children[2] = aml_data_node("\x00", 1); // ArgumentCount = 0

                if_zero->children[if_zero_index++] = external;
            }
            root->children[root_index++] = if_zero;
        }
        {
            // _HID for missing_hids
            uint16_t hid_index = 1;
            char hid_buffer[16];
            for (devpath_list *node = data.missing_hids; node != NULL; node = node->next) {
                sprintf(hid_buffer, "\x08_HID\x0d" "DIXY%04X\x00", hid_index++);
                aml_node *scope = aml_alloc_node(AML_OPCODE_ScopeOp, 2);
                // TODO: special HID for special devices like COM1
                scope->children[0] = aml_data_node(node->name, (uint16_t)node->name_size);
                scope->children[1] = aml_data_node(hid_buffer, 15);

                root->children[root_index++] = scope;
            }
        }
        {
            // _STA for offline_ddrcs
            for (devpath_list *node = data.offline_ddrcs; node != NULL; node = node->next) {
                aml_node *scope = aml_alloc_node(AML_OPCODE_ScopeOp, 2);
                scope->children[0] = aml_data_node(node->name, (uint16_t)node->name_size);
                scope->children[1] = aml_data_node("\x08_STA\x00", 6);

                root->children[root_index++] = scope;
            }
        }

        // serialize ssdt
        size_t ssdt_size = aml_node_size(root);
        char *ssdt_buffer = malloc(sizeof(EFI_ACPI_DESCRIPTION_HEADER) + ssdt_size);
        EFI_ACPI_DESCRIPTION_HEADER *header = (EFI_ACPI_DESCRIPTION_HEADER *)ssdt_buffer;
        header->Signature = 0x54445353 /* le 'SSDT' */;
        header->Length = (uint32_t)(sizeof(EFI_ACPI_DESCRIPTION_HEADER) + ssdt_size);
        header->Revision = 2;
        header->Checksum = 0; // let UEFI calculate checksum
        memcpy(header->OemId, "DIXYES", 6);
        memcpy(header->OemTableId, "DSDTFIX ", 8);
        header->OemRevision = 0x00000001;
        header->CreatorId = 0x59584944 /* le 'DIXY' */;
        header->CreatorRevision = 0x00000001;

        int32_t ret = aml_node_serialize(ssdt_buffer + sizeof(EFI_ACPI_DESCRIPTION_HEADER), 0, root);
        if (ret < 0) {
            printf("fix_dsdt: failed to serialize ssdt: %d\n", ret);
            free(ssdt_buffer);
            aml_free_node(root, 1);
            return 1;
        }

        // // debug
        // FILE *f = fopen("dsdt_fix.aml", "w");
        // if (f) {
        //     fwrite(ssdt_buffer, 1, sizeof(EFI_ACPI_DESCRIPTION_HEADER) + ssdt_size, f);
        //     fclose(f);
        // } else {
        //     printf("fix_dsdt: failed to write dsdt_fix_ssdt.aml: %d\n", errno);
        // }

        // install ssdt
        uintn_t table_key;
        efi_status_t status = acpi_table->InstallAcpiTable(
            acpi_table,
            ssdt_buffer,
            sizeof(EFI_ACPI_DESCRIPTION_HEADER) + ssdt_size,
            &table_key
        );
        if (EFI_SUCCESS != status) {
            printf("fix_dsdt: failed to install ssdt: %016x\n", status);
            free(ssdt_buffer);
            aml_free_node(root, 1);
            return 1;
        }

        free(ssdt_buffer);
        aml_free_node(root, 1);
    } else {
        printf("fix_dsdt: nothing to fix\n");
    }

    return 0;
}

// static const efi_guid_t acpi_proto_guid = EFI_ACPI_TABLE_PROTOCOL_GUID;

// int main(int argc, char **argv) {
//     EFI_ACPI_TABLE_PROTOCOL *acpi_table = NULL;

//     // get EFI_ACPI_TABLE_PROTOCOL
//     efi_status_t ret = BS->LocateProtocol((void*)&acpi_proto_guid, NULL, (void**)&acpi_table);
//     if (EFI_SUCCESS != ret || NULL == acpi_table) {
//         printf("failed LocateProtocol EFI_ACPI_TABLE_PROTOCOL: %d\n", ret);
//         return 1;
//     }

//     return fix_dsdt(acpi_table);
// }
