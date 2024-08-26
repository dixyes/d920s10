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
    efi_gop_t *gop;
    efi_status_t ret;
    uint32_t *pPixel;

    ret = BS->LocateProtocol((void*)&gop_guid, NULL, (void **)&gop);
    if (EFI_SUCCESS != ret || NULL == gop) {
        rprintf("failed LocateProtocol\n");
        return 1;
    }

    // write something to the screen
    printf("GOP: %p\n", gop);
    printf("Mode: %p\n", gop->Mode);
    printf("Mode->MaxMode: %d\n", gop->Mode->MaxMode);
    printf("Mode->Mode: %d\n", gop->Mode->Mode);
    printf("Version: %d\n", gop->Mode->Information->Version);
    printf("FrameBufferBase: %p\n", gop->Mode->FrameBufferBase);
    printf("FrameBufferSize: %08x\n", gop->Mode->FrameBufferSize);
    printf("HorizontalResolution: %d\n", gop->Mode->Information->HorizontalResolution);
    printf("VerticalResolution: %d\n", gop->Mode->Information->VerticalResolution);
    printf("PixelsPerScanLine: %d\n", gop->Mode->Information->PixelsPerScanLine);
    printf("PixelFormat: %08x\n", gop->Mode->Information->PixelFormat);
    printf("BlueMask: %08x\n", gop->Mode->Information->PixelInformation.BlueMask);
    printf("GreenMask: %08x\n", gop->Mode->Information->PixelInformation.GreenMask);
    printf("RedMask: %08x\n", gop->Mode->Information->PixelInformation.RedMask);
    // printf("Press enter key to write 0x66ccff00 to %p\n", gop->Mode->FrameBufferBase);
    // if (getchar() != '\r') {
    //     return 1;
    // }
    // pPixel = (void*)(uintptr_t)gop->Mode->FrameBufferBase;
    // for (int i = 0; i < gop->Mode->FrameBufferSize >> 2; i++) {
    //     pPixel[i] = 0x66ccff00;
    // }


    if (gop->Mode->FrameBufferBase < 0x80000000000LL) {
        // printf("Press enter key to write 0xffcc6600 to %p\n", gop->Mode->FrameBufferBase);
        // if (getchar() == '\r') {
        //     pPixel = (void*)(0x80000000000LL + (uintptr_t)gop->Mode->FrameBufferBase);
        //     for (int i = 0; i < gop->Mode->FrameBufferSize >> 2; i++) {
        //         pPixel[i] = 0xffcc6600;
        //     }
        // } else {
        //     printf("abort\n");
        // }
        pPixel = (void*)(0x80000000000LL + (uintptr_t)gop->Mode->FrameBufferBase);
        printf("modify the frame buffer base to %p\n", pPixel);
        gop->Mode->FrameBufferBase = (uint64_t)pPixel;
        // printf("modify format to 0\n");
        // gop->Mode->Information->PixelFormat = PixelRedGreenBlueReserved8BitPerColor;
        printf("done\n");
    }

    return 0;
}