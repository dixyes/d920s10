
#include "definitions.h"
#include "hob.h"

static const efi_guid_t hob_list_guid = EFI_HOB_LIST_GUID;
static const efi_guid_t hisi_memory_map_guid = EFI_MEMORY_MAP_GUID;

int hob_get_online_ddrcs (EFI_ACPI_TABLE_PROTOCOL *acpi_table, uintptr_t *online_ddrcs_base) {
    void *hobStart = NULL;
    GBL_INTERFACE *gbl = NULL;
    char *hob;
    EFI_HOB_GENERIC_HEADER *hdr;

    // find hob list
    for (uintn_t i = 0; i < ST->NumberOfTableEntries; i++) {
        if (!memcmp(
            &ST->ConfigurationTable[i].VendorGuid, &hob_list_guid,
            sizeof(efi_guid_t)
        )) {
            hobStart = ST->ConfigurationTable[i].VendorTable;
            break;
        }
    }

    if (!hobStart) {
        yprintf("hob_get_online_ddrcs: no hob list found\n");
        return 1;
    }

    // iterate hob list to find EFI_MEMORY_MAP_GUID
    hob = (char *)hobStart;
    hdr = (EFI_HOB_GENERIC_HEADER *)hob;

    while (hdr->HobType != 0xFFFF) {  // 0xFFFF = End of HOB List
        if (hdr->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
            EFI_HOB_GUID_TYPE *guidHob = (EFI_HOB_GUID_TYPE *)hdr;
            if (!memcmp(
                &guidHob->Name, &hisi_memory_map_guid,
                sizeof(efi_guid_t)
            )) {
                gbl = (GBL_INTERFACE *)(guidHob + 1);
                break;
            }
        }
        hob += hdr->HobLength;
        hdr = (EFI_HOB_GENERIC_HEADER *)hob;
    }

    if (!gbl) {
        yprintf("hob_get_online_ddrcs: gHisiEfiMemoryMapGuid HOB not found!\r\n");
        return 1;
    }

    // check online channels
    for (int socket = 0; socket < MAX_SOCKET; socket++) {
        for (int ch = 0; ch < MAX_CHANNEL; ch++) {
            uint8_t channel_online = 0;
            for (int dimm = 0; dimm < MAX_DIMM; dimm++) {
                DDR_DIMM_DATA *d = &gbl->Channel[socket][ch].Dimm[dimm];
                if (d->Enabled) {
                    channel_online = 1;
                    break;
                }
            }
            if (channel_online) {
                uintptr_t base;
                switch (socket) {
                    case 0:
                        base = 0x94d20000ULL;
                        break;
                    case 1:
                        base = 0x9cd20000ULL;
                        break;
                    case 2:
                        // not supported
                        base = 0x400094d20000ULL;
                        break;
                    case 3:
                        // not supported
                        base = 0x40009cd20000ULL;
                        break;
                    default:
                        // ??
                        abort();
                        return 1;
                }
                printf("hob_get_online_ddrcs: online socket %d channel %d at 0x%016lx\n", socket, ch, base + 0x10000ULL * (uintptr_t)ch);
                online_ddrcs_base[socket * MAX_CHANNEL + ch] = base + 0x10000ULL * (uintptr_t)ch;
            }
        }
    }

    return 0;
}