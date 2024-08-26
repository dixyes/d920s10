#include "definations.h"

const efi_guid_t acpi_guid = ACPI_TABLE_GUID;
const efi_guid_t acpi2_guid = ACPI_20_TABLE_GUID;
const efi_guid_t gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
const efi_guid_t acpi_proto_guid = EFI_ACPI_TABLE_PROTOCOL_GUID;
const efi_guid_t acpi_sdt_guid = EFI_ACPI_SDT_PROTOCOL_GUID;
const efi_guid_t smbios_guid = EFI_SMBIOS_PROTOCOL_GUID;
const efi_guid_t pciio_proto_guid = EFI_PCI_IO_PROTOCOL_GUID;

#define white() ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_GREEN | EFI_BLUE)
#define bright() ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE)
#define yellow() ST->ConOut->SetAttribute(ST->ConOut, EFI_YELLOW)
#define red() ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_BRIGHT)
#define green() ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BRIGHT)
#define yprintf(fmt, ...) do {\
    yellow(); printf(fmt, ##__VA_ARGS__); white(); \
} while (0)
#define rprintf(fmt, ...) do {\
    red(); printf(fmt, ##__VA_ARGS__); white(); \
} while (0)
#define gprintf(fmt, ...) do {\
    green(); printf(fmt, ##__VA_ARGS__); white(); \
} while (0)
#define brprintf(fmt, ...) do {\
    bright(); printf(fmt, ##__VA_ARGS__); white(); \
} while (0)


int main(int argc, char **argv) {
    efi_status_t ret;
    EFI_PCI_IO_PROTOCOL *pciio = NULL;
    EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *res;

    ret = BS->LocateProtocol((void*)&pciio_proto_guid, NULL, (void**)&pciio);
    if (EFI_SUCCESS != ret || NULL == pciio) {
        printf("failed LocateProtocol EFI_PCI_IO_PROTOCOL: %d\n", ret);
        return 1;
    }
    printf("PCI IO Protocol: %p\n", pciio);

    uint64_t handles_size = 0;
    efi_handle_t *handles = NULL;
    ret = BS->LocateHandle(ByProtocol, (void*)&pciio_proto_guid, NULL, &handles_size, handles);
    if (EFI_BUFFER_TOO_SMALL != ret) {
        printf("failed LocateHandle 1 EFI_PCI_IO_PROTOCOL: %d\n", ret);
        return 1;
    }
    ret = BS->AllocatePool(EfiBootServicesData, handles_size, (void*)&handles);
    if (EFI_SUCCESS != ret) {
        printf("failed AllocatePool: %d\n", ret);
        return 1;
    }
    ret = BS->LocateHandle(ByProtocol, (void*)&pciio_proto_guid, NULL, &handles_size, handles);
    if (EFI_SUCCESS != ret) {
        printf("failed LocateHandle 2 EFI_PCI_IO_PROTOCOL: %d\n", ret);
        return 1;
    }

    PCI_TYPE00 pci;

    for (uint64_t i = 0; i < handles_size / sizeof(efi_handle_t); i++) {
        printf("PCI IO Handle: %p\n", handles[i]);
        ret = BS->HandleProtocol(handles[i], (void*)&pciio_proto_guid, (void**)&pciio);
        if (EFI_SUCCESS != ret) {
            printf("failed HandleProtocol EFI_PCI_IO_PROTOCOL: %d\n", ret);
            continue;
        }

        ret = pciio->Pci.Read(pciio, EfiPciIoWidthUint8, 0, sizeof(pci), &pci);
        if (EFI_SUCCESS != ret) {
            printf("failed Read PCI: %d\n", ret);
            goto skip;
        }

        printf("PCI: %04x:%04x\n", pci.Hdr.VendorId, pci.Hdr.DeviceId);
        if (pci.Hdr.HeaderType != 0) {
            // is Switch or Root Complex virtual PCI Bridges
            goto skip;
        }

        for (int i = 0; i < 6; i++) {
            uint32_t bar = pci.Device.Bar[i];

            if (bar & 1) {
                printf("BAR %d: I/O %08x\n", i, bar & ~0x3);
                continue;
            }

            uint8_t type, prefetchable;
            uintptr_t baseAddress;

            type = (bar >> 1) & 3;
            prefetchable = (bar >> 3) & 1;
            if (type == 0) {
                // 32bit
                baseAddress = bar & ~0xf;
            } else {
                // 64bit
                baseAddress = bar & ~0xf;
                i++;
                bar = pci.Device.Bar[i];
                baseAddress |= (uint64_t)bar << 32;
            }
            printf("BAR %d: MEM type %d prefetchable %d address %08x\n",
                i, type, prefetchable, baseAddress);
        }

        if (pci.Hdr.VendorId == 0x1a03 && pci.Hdr.DeviceId == 0x2000) { // AST2500
            printf("fuck AST\n");
            // modify BARs address to 64 bit

            // reset BARS
            // printf("reset BARs\n");
            // char reset[24] = {0};
            // ret = pciio->Pci.Write(pciio, EfiPciIoWidthUint32, 0x10, sizeof(reset), &reset);
            // if (EFI_SUCCESS != ret) {
            //     printf("failed Write PCI: %d\n", ret);
            //     goto skip;
            // }

            uint64_t current = 0;
            printf("BAR 0: %08x\n", pci.Device.Bar[0]);
            printf("BAR 1: %08x\n", pci.Device.Bar[1]);
            printf("BAR 2: %08x\n", pci.Device.Bar[2]);
            printf("BAR 3: %08x\n", pci.Device.Bar[3]);
            printf("BAR 4: %08x\n", pci.Device.Bar[4]);
            printf("BAR 5: %08x\n", pci.Device.Bar[5]);

            current = pci.Device.Bar[4];

            // BAR 0
            printf("BAR 0: %08x\n", pci.Device.Bar[0]);

            current = pci.Device.Bar[0];
            current |= 0x80000000000LL; // make it 64 bit
            current |= 2 /* 0b10 */ << 1; // make it 64 bit

            printf("to write BAR 0: %016x\n", current);
            printf("press enter to modify BAR 0\n");
            if (getchar() != '\r') {
                return 1;
            }
            ret = pciio->Pci.Write(pciio, EfiPciIoWidthUint64, 0x10, 8, &current);
            if (EFI_SUCCESS != ret) {
                printf("failed Write PCI: %d\n", ret);
                goto skip;
            }
            printf("read BAR 0\n");
            ret = pciio->Pci.Read(pciio, EfiPciIoWidthUint64, 0x10, 8, &current);
            if (EFI_SUCCESS != ret) {
                printf("failed Read PCI: %d\n", ret);
                goto skip;
            }
            printf("BAR 0: %016x\n", current);

            // BAR 1 -> 2
            printf("BAR 2: %08x\n", pci.Device.Bar[2]);

            current = pci.Device.Bar[1];
            current |= 0x80000000000LL; // make it 64 bit
            current |= 2 /* 0b10 */ << 1; // make it 64 bit

            printf("to write BAR 2: %016x\n", current);
            printf("press enter to modify BAR 2\n");
            if (getchar() != '\r') {
                return 1;
            }
            ret = pciio->Pci.Write(pciio, EfiPciIoWidthUint64, 0x18, 8, &current);
            if (EFI_SUCCESS != ret) {
                printf("failed Write PCI: %d\n", ret);
                goto skip;
            }
            printf("read BAR 2\n");
            ret = pciio->Pci.Read(pciio, EfiPciIoWidthUint64, 0x18, 8, &current);
            if (EFI_SUCCESS != ret) {
                printf("failed Read PCI: %d\n", ret);
                goto skip;
            }
            printf("BAR 2: %016x\n", current);

            // BAR 2 -> 4 io
            printf("BAR 4: %08x\n", current);

            current = pci.Device.Bar[2];

            printf("to write BAR 4: %016x\n", current);
            printf("press enter to modify BAR 5\n");
            if (getchar() != '\r') {
                return 1;
            }
            ret = pciio->Pci.Write(pciio, EfiPciIoWidthUint64, 0x20, 8, &current);
            if (EFI_SUCCESS != ret) {
                printf("failed Write PCI: %d\n", ret);
                goto skip;
            }
            printf("read BAR 4\n");
            ret = pciio->Pci.Read(pciio, EfiPciIoWidthUint64, 0x20, 8, &current);
            if (EFI_SUCCESS != ret) {
                printf("failed Read PCI: %d\n", ret);
                goto skip;
            }
            printf("BAR 4: %016x\n", current);

        }


        // if (pci.Hdr.VendorId == 0x1ed5 && pci.Hdr.DeviceId == 0x0102) { // MTT
        // if (pci.Hdr.VendorId == 0x1a03 && pci.Hdr.DeviceId == 0x2000) { // AST2500
        //     printf("found AST\n");
        //     break;
        // }

        for (int i = 0; i < 6; i++) {
            if (pci.Device.Bar[i] & 1) {
                continue;
            }

            ret = pciio->GetBarAttributes(pciio, i, NULL, (void **)&res);
            if (EFI_SUCCESS != ret) {
                printf("BAR %d: failed GetBarAttributes: %d\n", i, ret);
                continue;
            }
            printf("BAR %d: %016x %016x\n", i, res->AddrRangeMin, res->AddrRangeMax);

            // if (pci.Hdr.VendorId == 0x1a03 && pci.Hdr.DeviceId == 0x2000) { // AST2500
            //     // modify BARs address
            //     if (res->AddrRangeMin < 0x80000000000LL) {
            //         res->AddrRangeMin += 0x80000000000LL;
            //         // TODO: fix this
                
            //         ret = pciio->GetBarAttributes(pciio, i, NULL, (void **)&res);
            //         if (EFI_SUCCESS != ret) {
            //             printf("mod BAR %d: failed GetBarAttributes: %d\n", i, ret);
            //             continue;
            //         }
            //         printf("mod BAR %d: %016x %016x\n", i, res->AddrRangeMin, res->AddrRangeMax);
            //     }
            // }
        }
        skip:
        printf("Press enter key to continue\n");
        if (getchar() != '\r') {
            return 1;
        }

        pciio = NULL;
    }
    return 0;
}