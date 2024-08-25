#include "definations.h"

// from util-linux
char *dmi_string(const EFI_SMBIOS_TABLE_HEADER *dm, uint8_t s)
{
	char *bp = (char *)dm;

	if (!s || !bp)
		return NULL;

	bp += dm->Length;
	while (s > 1 && *bp) {
		bp += strlen(bp);
		bp++;
		s--;
	}

	return !*bp ? NULL : bp;
}

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

#define PEU0_BASE 0x8002CB00000
#define PEU1_BASE 0x8002CB01000

// INTX 中断状态寄存器
#define PEU_INTX_INTR_STATUS_REG 0x000
// INTX 中断使能寄存器
#define PEU_INTX_INTR_ENABLE_REG 0x004
// 杂散中断状态寄存器
#define PEU_MISC_INTR_STATUS_REG 0x008
// 杂散中断使能寄存器
#define PEU_MISC_INTR_ENABLE_REG 0x00C
// 消息中断使能寄存器
#define PEU_MSG_INTR_ENABLE_REG 0x010
// 控制器错误状态寄存器
#define PEU_ERROR_STATUS_REG 0x0C0
// 控制器错误记录使能寄存器
#define PEU_ERROR_RECORD_ENABLE_REG 0x0C4
// 控制器错误中断使能寄存器
#define PEU_ERROR_INTR_ENABLE_REG 0x0C8
// 控制器错误写 1 置寄存器
#define PEU_ERROR_WRITE1_SET_REG 0x0CC
// 控制器错误写 1 清寄存器
#define PEU_ERROR_WRITE1_CLR_REG 0x0D0
// 控制器 MSI 使能寄存器
#define PEU_MSI_ENABLE_REG 0x200
// 控制器 MSI64 高位地址寄存器
#define PEU_MSI64_ADDR_HIGH_REG 0x208
// 控制器 MSI64 低位地址寄存器
#define PEU_MSI64_ADDR_LOW_REG 0x20C
// BIOS SRAM 校验使能寄存器
#define PEU_BIOS_SRAM_CHECK_ENABLE_REG 0x600
// VGA 使能寄存器
#define PEU_VGA_ENABLE_REG 0x700
// PIO 超时使能寄存器
#define PEU_PIO_TIMEOUT_ENABLE_REG 0x708
// PIO 写超时地址寄存器0
#define PEU_PIO_WRITE_TIMEOUT_ADDR0_REG 0x710
// PIO 写超时地址寄存器1
#define PEU_PIO_WRITE_TIMEOUT_ADDR1_REG 0x714
// PIO 读超时地址寄存器 0
#define PEU_PIO_READ_TIMEOUT_ADDR0_REG 0x718
// PIO 读超时地址寄存器 1
#define PEU_PIO_READ_TIMEOUT_ADDR1_REG 0x71c
// PEU 分拆模式寄存器
#define PEU_SPLIT_MODE_REG 0x800
// PEU 设备类型寄存器
#define PEU_DEV_TYPE_REG 0x804
// PEU 链路训练使能寄存器
#define PEU_LINK_TRAIN_ENABLE_REG 0x80C
// PEU 控制器链路数目设置寄存器
#define PEU_LINK_NUM_REG 0x838
// PEU 链路状态寄存器
#define PEU_LINK_STATUS_REG 0x83C
// 总线模式寄存器
#define PEU_BUS_MODE_REG 0xa20

uint32_t read32(uintptr_t addr);
void write32(uintptr_t addr, uint32_t val);

#define showreg0(regname) do { \
    ret = read32(PEU0_BASE + regname); \
    printf("PEU0 " #regname ": 0x%08x\n", regname); \
} while (0)

#define showreg2(regname) do { \
    ret = read32(PEU0_BASE + regname); \
    printf("PEU0 " #regname ": 0x%08x\n", ret); \
    ret = read32(PEU1_BASE + regname); \
    printf("PEU1 " #regname ": 0x%08x\n", ret); \
} while (0)

int main(int argc, char **argv) {
    intn_t ret;

    // show registers
    showreg0(PEU_INTX_INTR_STATUS_REG);
    showreg0(PEU_INTX_INTR_ENABLE_REG);
    showreg2(PEU_MISC_INTR_STATUS_REG);
    showreg2(PEU_MISC_INTR_ENABLE_REG);
    showreg2(PEU_MSG_INTR_ENABLE_REG);
    showreg2(PEU_ERROR_STATUS_REG);
    showreg2(PEU_ERROR_RECORD_ENABLE_REG);
    showreg2(PEU_ERROR_INTR_ENABLE_REG);
    showreg2(PEU_ERROR_WRITE1_SET_REG);
    showreg2(PEU_ERROR_WRITE1_CLR_REG);
    showreg2(PEU_MSI_ENABLE_REG);
    showreg2(PEU_MSI64_ADDR_HIGH_REG);
    showreg2(PEU_MSI64_ADDR_LOW_REG);
    showreg0(PEU_BIOS_SRAM_CHECK_ENABLE_REG);
    showreg2(PEU_VGA_ENABLE_REG);
    showreg2(PEU_PIO_TIMEOUT_ENABLE_REG);
    showreg2(PEU_PIO_WRITE_TIMEOUT_ADDR0_REG);
    showreg2(PEU_PIO_WRITE_TIMEOUT_ADDR1_REG);
    showreg2(PEU_PIO_READ_TIMEOUT_ADDR0_REG);
    showreg2(PEU_PIO_READ_TIMEOUT_ADDR1_REG);
    showreg2(PEU_SPLIT_MODE_REG);
    showreg2(PEU_DEV_TYPE_REG);
    showreg2(PEU_LINK_TRAIN_ENABLE_REG);
    showreg2(PEU_LINK_NUM_REG);
    showreg2(PEU_LINK_STATUS_REG);
    showreg2(PEU_BUS_MODE_REG);

    return 0;
}
