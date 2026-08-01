#include <uefi.h>

#define IN
#define OUT
typedef uintn_t EFI_STATUS;
typedef uint32_t UINT32;
typedef uint8_t UINT8;

#define HISI_SPI_FLASH_PROTOCOL_GUID {0x339132DC, 0xCED7, 0x4f84, {0xAA, 0xE7, 0x2E, 0xC4, 0xF9, 0x14, 0x38, 0x2F}}

typedef struct _HISI_SPI_FLASH_PROTOCOL HISI_SPI_FLASH_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_ERASE_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL   *This,
    IN UINT32                  Offset,
    IN UINT32                  ulLength
    );

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_WRITE_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL  *This,
    IN  UINT32                 Offset,
    IN  UINT8                 *Buffer,
    IN  UINT32                 ulLength
    );

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_READ_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL   *This,
    IN UINT32                  Offset,
    IN OUT UINT8              *Buffer,
    IN UINT32                  ulLength
    );

typedef
EFI_STATUS
(EFIAPI *HISI_SPI_FLASH_ERASE_WRITE_INTERFACE) (
    IN HISI_SPI_FLASH_PROTOCOL  *This,
    IN  UINT32                 Offset,
    IN  UINT8                 *Buffer,
    IN  UINT32                 ulLength
    );

struct _HISI_SPI_FLASH_PROTOCOL {
    HISI_SPI_FLASH_ERASE_INTERFACE             Erase;
    HISI_SPI_FLASH_WRITE_INTERFACE             Write;
    HISI_SPI_FLASH_READ_INTERFACE              Read;
    HISI_SPI_FLASH_ERASE_WRITE_INTERFACE       EraseWrite;
};

int main(int argc, const char **argv) {
    int ret;
    const efi_guid_t norGuid = HISI_SPI_FLASH_PROTOCOL_GUID;
    const char *dst = "dump.bin";
    if (argc > 1) {
        dst = argv[1];
    }

    HISI_SPI_FLASH_PROTOCOL *norp;
    ret = BS->LocateProtocol((void*)&norGuid, NULL, (void**)&norp);
    if (EFI_SUCCESS != ret) {
        printf("failed LocateProtocol HISI_SPI_FLASH_PROTOCOL: %d\n", ret);
        return 1;
    }
    printf("LocateProtocol HISI_SPI_FLASH_PROTOCOL success\n");

    FILE *fp = fopen(dst, "w");
    if (NULL == fp) {
        printf("failed open file %s: errno=%d\n", dst, errno);
        return 1;
    }

    unsigned char buf[4096];
    for (uint32_t offset = 0; offset < 16 * 1024 * 1024; offset += sizeof(buf)) {
        ret = norp->Read(norp, offset, buf, sizeof(buf));
        if (EFI_SUCCESS != ret) {
            printf("failed read nor: %d\n", ret);
            return 1;
        }
        fwrite(buf, 1, sizeof(buf), fp);
    }

    fflush(fp);
    fclose(fp);

    return 0;
}
