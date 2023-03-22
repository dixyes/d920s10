#include <uefi.h>

//extern efi_guid_t AcpiTableGuid; //?
#define EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION 0x02

typedef struct _EFI_ACPI_TABLE_PROTOCOL EFI_ACPI_TABLE_PROTOCOL;

typedef
efi_status_t
(EFIAPI *EFI_ACPI_TABLE_INSTALL_ACPI_TABLE)(
    EFI_ACPI_TABLE_PROTOCOL *This,
    void *AcpiTableBuffer,
    uintn_t AcpiTableBufferSize,
    uintn_t *TableKey
);

typedef
efi_status_t
(EFIAPI *EFI_ACPI_TABLE_UNINSTALL_ACPI_TABLE)(
    EFI_ACPI_TABLE_PROTOCOL *This,
    uintn_t TableKey
);

struct _EFI_ACPI_TABLE_PROTOCOL {
    EFI_ACPI_TABLE_INSTALL_ACPI_TABLE   InstallAcpiTable;
    EFI_ACPI_TABLE_UNINSTALL_ACPI_TABLE UninstallAcpiTable;
};


typedef struct {
    uint32_t    Signature;
    uint32_t    Length;
    uint8_t     Revision;
    uint8_t     Checksum;
    char     OemId[6];
    char     OemTableId[8];
    uint32_t    OemRevision;
    uint32_t    CreatorId;
    uint32_t    CreatorRevision;
} EFI_ACPI_SDT_HEADER;

typedef EFI_ACPI_SDT_HEADER EFI_ACPI_DESCRIPTION_HEADER;

#pragma pack(push,1)

typedef struct {
    uint64_t  Signature;
    uint8_t   Checksum;
    int8_t   OemId[6];
    uint8_t   Revision;
    uint32_t  RsdtAddress;
    uint32_t  Length;
    uint64_t  XsdtAddress;
    uint8_t   ExtendedChecksum;
    uint8_t   Reserved[3];
} EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER;


typedef struct {
    uint8_t Revision;
    uint16_t Length;
    uint8_t NumberofGenericAddressRegisters;
    uint16_t NameSpaceStringLength;
    uint16_t NameSpaceStringOffset;
    uint16_t OemDataLength;
    uint16_t OemDataOffset;
    uint16_t PortType;
    uint16_t PortSubtype;
    uint8_t Reserved[2];
    uint16_t BaseAddressRegisterOffset;
    uint16_t AddressSizeOffset;
} EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT;

typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER Header;
    uint32_t OffsetDbgDeviceInfo;
    uint32_t NumberDbgDeviceInfo;
} EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE;
const int a=sizeof(EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE);

typedef struct {
    uint8_t AddressSpaceId;
    uint8_t RegisterBitWidth;
    uint8_t RegisterBitOffset;
    uint8_t AccessSize;
    uint64_t Address;
} EFI_ACPI_6_1_GENERIC_ADDRESS_STRUCTURE;


#define NUMBER_DEBUG_DEVICE_INFO    1
#define NUMBER_OF_GENERIC_ADDRESS   1
#define NAMESPACE_STRING            "\\_SB.COM0"
#define NAMESPACE_STRING_SIZE       sizeof("\\_SB.COM0")
#define UART_LENGTH                 0x1000

typedef struct {
    EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT DdiHeader;
    EFI_ACPI_6_1_GENERIC_ADDRESS_STRUCTURE Address[NUMBER_OF_GENERIC_ADDRESS];
    uint32_t AddressSize[NUMBER_OF_GENERIC_ADDRESS];
    uint8_t  NamespaceString[NAMESPACE_STRING_SIZE];
} EFI_ACPI_DBG2_DDI_STRUCT;

typedef struct {
    EFI_ACPI_DEBUG_PORT_2_DESCRIPTION_TABLE Desc;
    EFI_ACPI_DBG2_DDI_STRUCT Ddi[NUMBER_DEBUG_DEVICE_INFO];
} EFI_ACPI_DEBUG_PORT_2_TABLE;


typedef struct {
    uint8_t   AddressSpaceId;
    uint8_t   RegisterBitWidth;
    uint8_t   RegisterBitOffset;
    uint8_t   AccessSize;
    uint64_t  Address;
} EFI_ACPI_5_0_GENERIC_ADDRESS_STRUCTURE;

typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER Header;
    uint8_t InterfaceType;
    uint8_t Reserved1[3];
    EFI_ACPI_5_0_GENERIC_ADDRESS_STRUCTURE  BaseAddress;
    uint8_t InterruptType;
    uint8_t Irq;
    uint32_t GlobalSystemInterrupt;
    uint8_t BaudRate;
    uint8_t Parity;
    uint8_t StopBits;
    uint8_t FlowControl;
    uint8_t TerminalType;
    uint8_t Reserved2;
    uint16_t PciDeviceId;
    uint16_t PciVendorId;
    uint8_t PciBusNumber;
    uint8_t PciDeviceNumber;
    uint8_t PciFunctionNumber;
    uint32_t PciFlags;
    uint8_t PciSegment;
    uint32_t Reserved3;
} EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE;

typedef uint32_t  EFI_ACPI_TABLE_VERSION;

typedef
efi_status_t
(EFIAPI *EFI_ACPI_GET_ACPI_TABLE2)(
  uintn_t                   Index,
  EFI_ACPI_SDT_HEADER     **Table,
  EFI_ACPI_TABLE_VERSION  *Version,
  uintn_t                   *TableKey
  );

#pragma pack(pop)

typedef struct _EFI_ACPI_SDT_PROTOCOL {
  ///
  /// A bit map containing all the ACPI versions supported by this protocol.
  ///
  EFI_ACPI_TABLE_VERSION      AcpiVersion;
  EFI_ACPI_GET_ACPI_TABLE2      GetAcpiTable;
//   EFI_ACPI_REGISTER_NOTIFY    RegisterNotify;
//   EFI_ACPI_OPEN               Open;
//   EFI_ACPI_OPEN_SDT           OpenSdt;
//   EFI_ACPI_CLOSE              Close;
//   EFI_ACPI_GET_CHILD          GetChild;
//   EFI_ACPI_GET_OPTION         GetOption;
//   EFI_ACPI_SET_OPTION         SetOption;
//   EFI_ACPI_FIND_PATH          FindPath;
} EFI_ACPI_SDT_PROTOCOL;

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_ARM_OEM_ID           'H','I','S','I',' ',' '   // OEMID 6 bytes long
#define EFI_ACPI_ARM_OEM_TABLE_ID     SIGNATURE_64 ('H','I','P','0','8',' ',' ',' ') // OEM table id 8 bytes long
#define EFI_ACPI_ARM_OEM_REVISION     0x00000000
#define EFI_ACPI_ARM_CREATOR_ID       SIGNATURE_32 ('H','I','S','I')
#define EFI_ACPI_ARM_CREATOR_REVISION 0x20151124

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define ARM_ACPI_HEADER(Signature, Type, Revision) {            \
  Signature,                      /* UINT32  Signature */       \
  sizeof (Type),                  /* UINT32  Length */          \
  Revision,                       /* UINT8   Revision */        \
  0,                              /* UINT8   Checksum */        \
  { EFI_ACPI_ARM_OEM_ID },        /* UINT8   OemId[6] */        \
  { 'H','I','P','0','8',' ',' ',' ' }/*EFI_ACPI_ARM_OEM_TABLE_ID*/,      /* UINT64  OemTableId */      \
  EFI_ACPI_ARM_OEM_REVISION,      /* UINT32  OemRevision */     \
  0x49534948/*EFI_ACPI_ARM_CREATOR_ID*/,        /* UINT32  CreatorId */       \
  EFI_ACPI_ARM_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }


#if __GNUC__ > 3
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t)( (char *)&(((type *)0)->member) - (char *)0 ))
#endif

#define EFI_ACPI_DBG2_PORT_TYPE_SERIAL                                                 0x8000
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_FULL_16550                                 0x0000
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_16550_SUBSET_COMPATIBLE_WITH_MS_DBGP_SPEC  0x0001
#define EFI_ACPI_DBG2_PORT_TYPE_1394                                                   0x8001
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_1394_STANDARD                                     0x0000
#define EFI_ACPI_DBG2_PORT_TYPE_USB                                                    0x8002
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_USB_XHCI                                          0x0000
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_USB_EHCI                                          0x0001
#define EFI_ACPI_DBG2_PORT_TYPE_NET                                                    0x8003

#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_PL011_UART                             0x0003
#define EFI_ACPI_RESERVED_BYTE  0x00
#define EFI_ACPI_RESERVED_DWORD  0x00000000

#define EFI_ACPI_6_1_SYSTEM_MEMORY              0

#define EFI_ACPI_6_1_BYTE       1

#define EFI_ACPI_SDT_PROTOCOL_GUID \
    { 0xeb97088e, 0xcfdf, 0x49c6, { 0xbe, 0x4b, 0xd9, 0x6, 0xa5, 0xb2, 0xe, 0x86 }}

#define EFI_ACPI_TABLE_PROTOCOL_GUID \
    { \
        0xffe06bdd, 0x6107, 0x46a6, {0x7b, 0xb2, 0x5a, 0x9c, 0x7e, 0xc5, 0x27, 0x5c} \
    }
#define EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID \
    { \
        0x5c99a21, 0xc70f, 0x4ad2, {0x8a, 0x5f, 0x35, 0xdf, 0x33, 0x43, 0xf5, 0x1e  } \
    }
#define EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID \
    { \
        0x8b843e20, 0x8132, 0x4852, {0x90, 0xcc, 0x55, 0x1a, 0x4e, 0x4a, 0x7f, 0x1c } \
    }
#define EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID \
    { \
        0x379be4e, 0xd706, 0x437d, {0xb0, 0x37, 0xed, 0xb8, 0x2f, 0xb7, 0x72, 0xa4 } \
    }

typedef struct {
  uint8_t    Type;
  uint8_t    SubType;
  uint8_t    Length[2];
} EFI_DEVICE_PATH_PROTOCOL;


typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_NODE)(
  const wchar_t                 *TextDeviceNode
  );

typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_PATH)(
  const wchar_t                 *TextDevicePath
  );

typedef struct {
  EFI_DEVICE_PATH_FROM_TEXT_NODE    ConvertTextToDeviceNode;
  EFI_DEVICE_PATH_FROM_TEXT_PATH    ConvertTextToDevicePath;
} EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

typedef
wchar_t *
(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_NODE)(
  const EFI_DEVICE_PATH_PROTOCOL   *DeviceNode,
  boolean_t                          DisplayOnly,
  boolean_t                          AllowShortcuts
  );

typedef
wchar_t *
(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_PATH)(
  const EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
  boolean_t                          DisplayOnly,
  boolean_t                          AllowShortcuts
  );

typedef struct {
  EFI_DEVICE_PATH_TO_TEXT_NODE    ConvertDeviceNodeToText;
  EFI_DEVICE_PATH_TO_TEXT_PATH    ConvertDevicePathToText;
} EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

 
typedef
uintn_t
(EFIAPI *EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE)(
  const EFI_DEVICE_PATH_PROTOCOL *DevicePath
  );
 
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH)(
  const EFI_DEVICE_PATH_PROTOCOL *DevicePath
  );
 
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_PATH)(
  const EFI_DEVICE_PATH_PROTOCOL *Src1,
  const EFI_DEVICE_PATH_PROTOCOL *Src2
  );
 
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_NODE)(
  const EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  const EFI_DEVICE_PATH_PROTOCOL *DeviceNode
  );
 
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE)(
  const EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  const EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance
  );
 
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE)(
  EFI_DEVICE_PATH_PROTOCOL  **DevicePathInstance,
  uintn_t                         *DevicePathInstanceSize
  );
 
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_CREATE_NODE)(
  uint8_t                          NodeType,
  uint8_t                          NodeSubType,
  uint16_t                         NodeLength
  );
 
typedef
boolean_t
(EFIAPI *EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE)(
  const EFI_DEVICE_PATH_PROTOCOL         *DevicePath
  );
 
typedef struct {
  EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE    GetDevicePathSize;
  EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH         DuplicateDevicePath;
  EFI_DEVICE_PATH_UTILS_APPEND_PATH             AppendDevicePath;
  EFI_DEVICE_PATH_UTILS_APPEND_NODE             AppendDeviceNode;
  EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE         AppendDevicePathInstance;
  EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE       GetNextDevicePathInstance;
  EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE       IsDevicePathMultiInstance;
  EFI_DEVICE_PATH_UTILS_CREATE_NODE             CreateDeviceNode;
} EFI_DEVICE_PATH_UTILITIES_PROTOCOL;



int install_dbg2(EFI_ACPI_TABLE_PROTOCOL *acpi_table) {
    uintn_t table_key;

    EFI_ACPI_DEBUG_PORT_2_TABLE Dbg2 = {
        {
            ARM_ACPI_HEADER(
                0x32474244 /*EFI_ACPI_6_1_DEBUG_PORT_2_TABLE_SIGNATURE, "DBG2" in le*/,
                EFI_ACPI_DEBUG_PORT_2_TABLE,
                0 /*EFI_ACPI_DEBUG_PORT_2_TABLE_REVISION*/
                ),
            offsetof(EFI_ACPI_DEBUG_PORT_2_TABLE, Ddi),
            NUMBER_DEBUG_DEVICE_INFO
        },
        {
            {
                {
                    0 /*EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT_REVISION*/,
                    sizeof(EFI_ACPI_DBG2_DDI_STRUCT),
                    NUMBER_OF_GENERIC_ADDRESS,
                    NAMESPACE_STRING_SIZE,
                    offsetof(EFI_ACPI_DBG2_DDI_STRUCT, NamespaceString),
                    0,  //OemDataLength
                    0,  //OemDataOffset
                    EFI_ACPI_DBG2_PORT_TYPE_SERIAL,
                    EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_PL011_UART,
                    {EFI_ACPI_RESERVED_BYTE, EFI_ACPI_RESERVED_BYTE},
                    offsetof(EFI_ACPI_DBG2_DDI_STRUCT, Address),
                    offsetof(EFI_ACPI_DBG2_DDI_STRUCT, AddressSize),
                },
                {
                    {
                    EFI_ACPI_6_1_SYSTEM_MEMORY,
                    32,
                    0,
                    EFI_ACPI_6_1_BYTE,
                    //0x400094080000/*FixedPcdGet64 (PcdSerialDbgRegisterBase)*/
                    0x94080000L/*FixedPcdGet64 (PcdSerialDbgRegisterBase)*/
                    }
                },
            {
                UART_LENGTH
            },
            NAMESPACE_STRING
            }
        }
    };

    efi_status_t ret = acpi_table->InstallAcpiTable(acpi_table, &Dbg2, sizeof(Dbg2), &table_key);
    if (EFI_SUCCESS != ret) {
        printf("failed InstallAcpiTable DBG2\n");
        return 1;
    }
    printf("DBG2 registered %d\n", table_key);

    return 0;
}

int install_spcr(EFI_ACPI_TABLE_PROTOCOL *acpi_table) {
    uintn_t table_key;
    
    EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE Spcr = {
        ARM_ACPI_HEADER ( 0x52435053 /* EFI_ACPI_6_2_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_SIGNATURE, "SPCR" in le */,
                        EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE,
                        0x02 /*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_REVISION*/),
        // UINT8                                   InterfaceType;
        0x03 /*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_PL011_UART*/,
        // UINT8                                   Reserved1[3];
        {
        EFI_ACPI_RESERVED_BYTE,
        EFI_ACPI_RESERVED_BYTE,
        EFI_ACPI_RESERVED_BYTE
        },
        // EFI_ACPI_6_2_GENERIC_ADDRESS_STRUCTURE  BaseAddress;
        { 0/*EFI_ACPI_5_0_SYSTEM_MEMORY*/, 32, 0, 3/*EFI_ACPI_5_0_DWORD*/, 0x94080000 }/*ARM_GAS32 (FixedPcdGet64 (PcdSerialRegisterBase))*/,
        // UINT8                                   InterruptType;
        0x8 /*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERRUPT_TYPE_GIC*/,
        // UINT8                                   Irq;
        0,                                         // Not used on ARM
        // UINT32                                  GlobalSystemInterrupt;
        141,
        // UINT8                                   BaudRate;
        7/*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_BAUD_RATE_115200*/,
        // UINT8                                   Parity;
        0/*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_PARITY_NO_PARITY*/,
        // UINT8                                   StopBits;
        1/*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_STOP_BITS_1*/,
        // UINT8                                   FlowControl;
        0/*SPCR_FLOW_CONTROL_NONE*/,
        // UINT8                                   TerminalType;
        3 /*EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_TERMINAL_TYPE_ANSI*/,
        // UINT8                                   Reserved2;
        EFI_ACPI_RESERVED_BYTE,
        // UINT16                                  PciDeviceId;
        0xFFFF,
        // UINT16                                  PciVendorId;
        0xFFFF,
        // UINT8                                   PciBusNumber;
        0x00,
        // UINT8                                   PciDeviceNumber;
        0x00,
        // UINT8                                   PciFunctionNumber;
        0x00,
        // UINT32                                  PciFlags;
        0x00000000,
        // UINT8                                   PciSegment;
        0x00,
        // UINT32                                  Reserved3;
        EFI_ACPI_RESERVED_DWORD
    };

    efi_status_t ret = acpi_table->InstallAcpiTable(acpi_table, &Spcr, sizeof(Spcr), &table_key);
    if (EFI_SUCCESS != ret) {
        printf("failed InstallAcpiTable SPCR\n");
        return 1;
    }
    printf("SPCR registered %d\n", table_key);
    return 0;
}

int fix_rtc0_hid (EFI_ACPI_TABLE_PROTOCOL *acpi_table) {
    uintn_t table_key;

    char ssdt[] = {
        0x53, 0x53, 0x44, 0x54, 0x59, 0x00, 0x00, 0x00, 0x02, 0x95, 0x44, 0x49,
        0x58, 0x59, 0x45, 0x53, 0x44, 0x49, 0x58, 0x59, 0x45, 0x53, 0x30, 0x30,
        0x00, 0x00, 0x00, 0x00, 0x49, 0x4e, 0x54, 0x4c, 0x20, 0x10, 0x22, 0x20,
        0xa0, 0x14, 0x00, 0x15, 0x5c, 0x2f, 0x03, 0x5f, 0x53, 0x42, 0x5f, 0x49,
        0x32, 0x43, 0x34, 0x52, 0x54, 0x43, 0x30, 0x06, 0x00, 0x10, 0x1f, 0x5c,
        0x2f, 0x03, 0x5f, 0x53, 0x42, 0x5f, 0x49, 0x32, 0x43, 0x34, 0x52, 0x54,
        0x43, 0x30, 0x08, 0x5f, 0x48, 0x49, 0x44, 0x0d, 0x44, 0x49, 0x58, 0x59,
        0x30, 0x30, 0x30, 0x30, 0x00
    };

    efi_status_t ret = acpi_table->InstallAcpiTable(acpi_table, &ssdt, sizeof(ssdt), &table_key);
    if (EFI_SUCCESS != ret) {
        printf("failed InstallAcpiTable SSDT fix for RTC0 _HID\n");
        return 1;
    }
    printf("SSDT fix for RTC0 _HID registered %d\n", table_key);
    return 0;
}

int uinstall_pcct(EFI_ACPI_TABLE_PROTOCOL *acpi_table, EFI_ACPI_SDT_PROTOCOL *acpi_sdt) {
    efi_status_t ret;
    EFI_ACPI_SDT_HEADER *pTable = NULL;
    EFI_ACPI_TABLE_VERSION dummy;
    uintn_t key;
    char sign[5] = { 0 };
    for (int i = 0; i < 256; i++) {
        ret = acpi_sdt->GetAcpiTable(i, &pTable, &dummy, &key);
        if (EFI_SUCCESS != ret) {
            printf("not found %d\n", i);
            break;
        }
        memcpy(sign, &pTable->Signature, 4);
        printf("found \"%s\" at %d\n", sign, i);

        if (pTable->Signature == 0x54434350) {
            printf("uninstall PCCT at %d\n", i);

            // pTable->Signature = 0x584d454f;
            
            ret = acpi_table->UninstallAcpiTable(acpi_table, key);
            if (EFI_SUCCESS != ret) {
                printf("failed UninstallAcpiTable PCCT\n");
                return 1;
            }
            break;
        }
    }
    return 0;
}

int set_fhd(efi_gop_t *gop) {
    efi_status_t ret;
    
    uint64_t max_pixels = 0;
    uintn_t isiz = sizeof(efi_gop_mode_info_t);
    efi_gop_mode_info_t *info = NULL;
    for (uintn_t try_mode = gop->Mode->MaxMode - 1; try_mode >= 0; try_mode--) {
        ret = gop->QueryMode(gop, try_mode, &isiz, &info);
        if (EFI_ERROR(ret) || info->PixelFormat > PixelBitMask) {
            // unsupported
            continue;
        }

        if (info->VerticalResolution * info->HorizontalResolution <= max_pixels){
            continue;
        }
        max_pixels = info->VerticalResolution * info->HorizontalResolution;

        printf("setting to %d: %dx%d\n", try_mode, info->HorizontalResolution, info->VerticalResolution);
        ret = gop->SetMode(gop, try_mode);
        if (EFI_ERROR(ret)) {
            fprintf(stderr, "unable to set video mode: %d\n", ret);
            return 1;
        }
        if (
            (info->HorizontalResolution == 3840 && info->VerticalResolution == 2160 ) ||
            (info->HorizontalResolution == 2560 && info->VerticalResolution == 1600 ) ||
            (info->HorizontalResolution == 2560 && info->VerticalResolution == 1440 ) ||
            (info->HorizontalResolution == 1920 && info->VerticalResolution == 1080 ) ||
            (info->HorizontalResolution == 1280 && info->VerticalResolution == 720 ) ||
            0
        ) {
            // prefered
            break;
        }
    }
    return 0;
}

int chainload_bootmgr() {
    efi_status_t ret;

    printf("chainload bootmgr\n");

    const char_t *try_paths[] = {
        "\\EFI\\Microsoft\\Boot\\bootmgfw.efi",
        "\\EFI\\Microsoft\\Boot\\cdboot_noprompt.efi",
        "\\EFI\\Microsoft\\Boot\\bootmgr.efi"
    };

    FILE *dummy;
    int i = 0;
    for (; try_paths[i] != NULL; i++) {
        dummy = fopen(try_paths[i], "*");
        if (dummy) {
            fclose(dummy);
            break;
        }
    }

    if (!dummy) {
        printf("failed to find bootmgfw.efi, cannot chainload\n");
        return 1;
    }

    efi_handle_t handle;
    wchar_t wpath[64];
    mbstowcs(wpath, try_paths[i], 64);

    const efi_guid_t dpftpGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
    EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *dpftp;
    ret = BS->LocateProtocol((void*)&dpftpGuid, NULL, (void**)&dpftp);
    if (EFI_SUCCESS != ret) {
        printf("failed LocateProtocol EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL: %d\n", ret);
        return 1;
    }

    const efi_guid_t dpttpGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *dpttp;
    ret = BS->LocateProtocol((void*)&dpttpGuid, NULL, (void**)&dpttp);
    if (EFI_SUCCESS != ret) {
        printf("failed LocateProtocol EFI_DEVICE_PATH_TO_TEXT_PROTOCOL: %d\n", ret);
        return 1;
    }
    
    const efi_guid_t dpupGuid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
    EFI_DEVICE_PATH_UTILITIES_PROTOCOL *dpup;
    ret = BS->LocateProtocol((void*)&dpupGuid, NULL, (void**)&dpup);
    if (EFI_SUCCESS != ret) {
        printf("failed LocateProtocol EFI_DEVICE_PATH_UTILITIES_PROTOCOL: %d\n", ret);
        return 1;
    }

    char buf[64];

    EFI_DEVICE_PATH_PROTOCOL *dev_node;
    const efi_guid_t dppGuid = EFI_DEVICE_PATH_PROTOCOL_GUID;
    ret = BS->OpenProtocol(
        LIP->DeviceHandle,
        (void *)&dppGuid,
        (void **)&dev_node,
        IM,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if (EFI_SUCCESS != ret) {
        printf("failed OpenProtocol: %d\n", ret);
        return 1;
    }
    // wcstombs(buf, dpttp->ConvertDeviceNodeToText(dev_node, 0, 0), 64);
    // printf("cao %s\n", buf);

    EFI_DEVICE_PATH_PROTOCOL *dev_path = dpftp->ConvertTextToDevicePath(wpath);
    if (!dev_path) {
        printf("failed ConvertTextToDevicePath\n");
        return 1;
    }
    // wcstombs(buf, dpttp->ConvertDevicePathToText(dev_path, 0, 0), 64);
    // printf("cao2 %s\n", buf);

    dev_path = dpup->AppendDevicePath(dev_node, dev_path);
    if (!dev_path) {
        printf("failed AppendDevicePath\n");
        return 1;
    }
    wcstombs(buf, dpttp->ConvertDevicePathToText(dev_path, 0, 0), 64);
    printf("load %s\n", buf);

    ret = BS->LoadImage(0, IM, (void*)dev_path, NULL, 0, &handle);
    if (EFI_SUCCESS != ret) {
        printf("failed LoadImage %s: %d\n", try_paths[i], ret);
        return 1;
    }

    printf("start %s\n", try_paths[i]);
    ret = BS->StartImage(handle, NULL, NULL);
    if (EFI_SUCCESS != ret) {
        printf("failed StartImage %s: %d\n", try_paths[i], ret);
        return 1;
    }

    return 0;
}

int main(int argc, char **argv) {

    efi_status_t ret;
    const efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_gop_t *gop = NULL;
    ret = BS->LocateProtocol((void*)&gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(ret) || !gop) {
        fprintf(stderr, "failed LocateProtocol EFI_GRAPHICS_OUTPUT_PROTOCOL: %d\n", ret);
    } else {
        // set resolution to 1080
        set_fhd(gop);
    }

    // get EFI_ACPI_TABLE_PROTOCOL
    const efi_guid_t acpi_proto_guid = EFI_ACPI_TABLE_PROTOCOL_GUID;
    EFI_ACPI_TABLE_PROTOCOL *acpi_table;
    ret = BS->LocateProtocol((void*)&acpi_proto_guid, NULL, (void**)&acpi_table);
    if (EFI_SUCCESS != ret || NULL == acpi_table) {
        printf("failed LocateProtocol EFI_ACPI_TABLE_PROTOCOL: %d\n", ret);
        return 1;
    }

    // register dbg2 for debug use
    install_dbg2(acpi_table);

    // register spcr for debug use
    install_spcr(acpi_table);

    // fix missing _HID for RTC0
    fix_rtc0_hid(acpi_table);

    // get EFI_ACPI_SDT_PROTOCOL
    const efi_guid_t acpi_sdt_guid = EFI_ACPI_SDT_PROTOCOL_GUID;
    EFI_ACPI_SDT_PROTOCOL *acpi_sdt;
    ret = BS->LocateProtocol((void*)&acpi_sdt_guid, NULL, (void **)&acpi_sdt);
    if (EFI_SUCCESS != ret || NULL == acpi_sdt) {
        printf("failed LocateProtocol EFI_ACPI_SDT_PROTOCOL: %d\n", ret);
    } else {
        // uninstall wrong pcct
        uinstall_pcct(acpi_table, acpi_sdt);
    }

    // chainload
    if (argc == 0 || (argc > 1 && !strcmp(argv[1], "chainload"))) {
        chainload_bootmgr();
    }

    return 0;
}
