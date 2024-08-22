#include <uefi.h>

#define EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION 0x02

/* GUIDs */

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
#define EFI_SMBIOS_PROTOCOL_GUID \
    { 0x3583ff6, 0xcb36, 0x4940, { 0x94, 0x7e, 0xb9, 0xb3, 0x9f, 0x4a, 0xfa, 0xf7 }}
#define EFI_PCI_IO_PROTOCOL_GUID \
    { 0x4cf5b200, 0x68b8, 0x4ca5, { 0x9e, 0xec, 0xb2, 0x3e, 0x3f, 0x50, 0x2, 0x9a }}

/* acpi protocol */

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

/* sdt structure and protocol */

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
    /* not used */
    // EFI_ACPI_REGISTER_NOTIFY    RegisterNotify;
    // EFI_ACPI_OPEN               Open;
    // EFI_ACPI_OPEN_SDT           OpenSdt;
    // EFI_ACPI_CLOSE              Close;
    // EFI_ACPI_GET_CHILD          GetChild;
    // EFI_ACPI_GET_OPTION         GetOption;
    // EFI_ACPI_SET_OPTION         SetOption;
    // EFI_ACPI_FIND_PATH          FindPath;
} EFI_ACPI_SDT_PROTOCOL;


/* SPCR/DBG2 constants and struct */

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
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_PL011_UART                             0x0003
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_NVIDIA_16550_UART                          0x0005
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_SBSA_GENERIC_UART_2X                   0x000d
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_SBSA_GENERIC_UART                      0x000e
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_DCC                                        0x000f
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_BCM2835_UART                               0x0010
#define   EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_16550_WITH_GAS                             0x0012
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
#define EFI_ACPI_6_1_DWORD      3

/* device path protocol */

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

typedef struct _EFI_SMBIOS_PROTOCOL EFI_SMBIOS_PROTOCOL;

typedef uint16_t EFI_SMBIOS_HANDLE;
typedef uint8_t EFI_SMBIOS_TYPE;
typedef struct {
    EFI_SMBIOS_TYPE Type;
    uint8_t Length;
    EFI_SMBIOS_HANDLE Handle;
} EFI_SMBIOS_TABLE_HEADER;

typedef uintn_t (EFIAPI *EFI_SMBIOS_ADD) (
    const EFI_SMBIOS_PROTOCOL     *This,
    efi_handle_t ProducerHandle,
    EFI_SMBIOS_HANDLE *SmbiosHandle,
    EFI_SMBIOS_TABLE_HEADER *Record
);

typedef uintn_t(EFIAPI *EFI_SMBIOS_UPDATE_STRING)(
    const EFI_SMBIOS_PROTOCOL *This,
    EFI_SMBIOS_HANDLE *SmbiosHandle,
    uintn_t *StringNumber,
    int8_t *String
);

typedef uintn_t(EFIAPI *EFI_SMBIOS_REMOVE)(
    const EFI_SMBIOS_PROTOCOL *This,
    EFI_SMBIOS_HANDLE SmbiosHandle
);

typedef uintn_t(EFIAPI *EFI_SMBIOS_GET_NEXT)(
    const EFI_SMBIOS_PROTOCOL *This,
    EFI_SMBIOS_HANDLE *SmbiosHandle,
    EFI_SMBIOS_TYPE *Type,
    EFI_SMBIOS_TABLE_HEADER **Record,
    efi_handle_t *ProducerHandle
);

struct _EFI_SMBIOS_PROTOCOL {
    EFI_SMBIOS_ADD Add;
    EFI_SMBIOS_UPDATE_STRING UpdateString;
    EFI_SMBIOS_REMOVE Remove;
    EFI_SMBIOS_GET_NEXT GetNext;
    uint8_t MajorVersion;
    uint8_t MinorVersion;
};
typedef struct {
    uint16_t Size : 14;
    uint16_t Unit : 2;
} EXTENDED_BIOS_ROM_SIZE;

#pragma pack(push,1)
/* SMBIOS structure */
typedef struct {
    EFI_SMBIOS_TABLE_HEADER Hdr;
    uint8_t Vendor;
    uint8_t BiosVersion;
    uint16_t BiosSegment;
    uint8_t BiosReleaseDate;
    uint8_t BiosSize;
    uint32_t BiosCharacteristics;
    uint8_t BIOSCharacteristicsExtensionBytes[2];
    uint8_t SystemBiosMajorRelease;
    uint8_t SystemBiosMinorRelease;
    uint8_t EmbeddedControllerFirmwareMajorRelease;
    uint8_t EmbeddedControllerFirmwareMinorRelease;
    //
    // Add for smbios 3.1.0
    //
    EXTENDED_BIOS_ROM_SIZE       ExtendedBiosSize;
} SMBIOS_TABLE_TYPE0;

typedef struct {
    EFI_SMBIOS_TABLE_HEADER Hdr;
    uint8_t Manufacturer;
    uint8_t ProductName;
    uint8_t Version;
    uint8_t SerialNumber;
    efi_guid_t Uuid;
    uint8_t WakeUpType; ///< The enumeration value from MISC_SYSTEM_WAKEUP_TYPE.
    uint8_t SKUNumber;
    uint8_t Family;
} SMBIOS_TABLE_TYPE1;

/* pciio protocol */

typedef struct _EFI_PCI_IO_PROTOCOL EFI_PCI_IO_PROTOCOL;

typedef enum {
    EfiPciIoWidthUint8 = 0,
    EfiPciIoWidthUint16,
    EfiPciIoWidthUint32,
    EfiPciIoWidthUint64,
    EfiPciIoWidthFifoUint8,
    EfiPciIoWidthFifoUint16,
    EfiPciIoWidthFifoUint32,
    EfiPciIoWidthFifoUint64,
    EfiPciIoWidthFillUint8,
    EfiPciIoWidthFillUint16,
    EfiPciIoWidthFillUint32,
    EfiPciIoWidthFillUint64,
    EfiPciIoWidthMaximum
} EFI_PCI_IO_PROTOCOL_WIDTH;

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_POLL_IO_MEM)(
    EFI_PCI_IO_PROTOCOL *This,
    EFI_PCI_IO_PROTOCOL_WIDTH Width,
    uint8_t BarIndex,
    uint64_t Offset,
    uint64_t Mask,
    uint64_t Value,
    uint64_t Delay,
    uint64_t *Result
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_IO_MEM)(
    EFI_PCI_IO_PROTOCOL *This,
    EFI_PCI_IO_PROTOCOL_WIDTH Width,
    uint8_t BarIndex,
    uint64_t Offset,
    uintn_t Count,
    void *Buffer
);

typedef struct {
    EFI_PCI_IO_PROTOCOL_IO_MEM Read;
    EFI_PCI_IO_PROTOCOL_IO_MEM Write;
} EFI_PCI_IO_PROTOCOL_ACCESS;

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_CONFIG)(
    EFI_PCI_IO_PROTOCOL *This,
    EFI_PCI_IO_PROTOCOL_WIDTH Width,
    uint32_t Offset,
    uintn_t Count,
    void *Buffer
);

typedef struct {
    EFI_PCI_IO_PROTOCOL_CONFIG Read;
    EFI_PCI_IO_PROTOCOL_CONFIG Write;
} EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS;

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_COPY_MEM)(
    EFI_PCI_IO_PROTOCOL *This,
    EFI_PCI_IO_PROTOCOL_WIDTH Width,
    uint8_t DestBarIndex,
    uint64_t DestOffset,
    uint8_t SrcBarIndex,
    uint64_t SrcOffset,
    uintn_t Count
);

typedef enum {
    EfiPciIoOperationBusMasterRead,
    EfiPciIoOperationBusMasterWrite,
    EfiPciIoOperationBusMasterCommonBuffer,
    EfiPciIoOperationMaximum
} EFI_PCI_IO_PROTOCOL_OPERATION;

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_MAP)(
    EFI_PCI_IO_PROTOCOL *This,
    EFI_PCI_IO_PROTOCOL_OPERATION Operation,
    void *HostAddress,
    uintn_t *NumberOfBytes,
    uint64_t *DeviceAddress,
    void **Mapping
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_UNMAP)(
    EFI_PCI_IO_PROTOCOL *This,
    void *Mapping
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER)(
    EFI_PCI_IO_PROTOCOL *This,
    efi_allocate_type_t Type,
    efi_memory_type_t MemoryType,
    uintn_t Pages,
    void **HostAddress,
    uint64_t Attributes
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_FREE_BUFFER)(
    EFI_PCI_IO_PROTOCOL *This,
    uintn_t Pages,
    void *HostAddress
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_FLUSH)(
    EFI_PCI_IO_PROTOCOL *This
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_GET_LOCATION)(
    EFI_PCI_IO_PROTOCOL *This,
    uintn_t *SegmentNumber,
    uintn_t *BusNumber,
    uintn_t *DeviceNumber,
    uintn_t *FunctionNumber
);

typedef enum {
    EfiPciIoAttributeOperationGet,
    EfiPciIoAttributeOperationSet,
    EfiPciIoAttributeOperationEnable,
    EfiPciIoAttributeOperationDisable,
    EfiPciIoAttributeOperationSupported,
    EfiPciIoAttributeOperationMaximum
} EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION;

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_ATTRIBUTES)(
    EFI_PCI_IO_PROTOCOL *This,
    EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
    uint64_t Attributes,
    uint64_t *Result
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES)(
    EFI_PCI_IO_PROTOCOL *This,
    uint8_t BarIndex,
    uint64_t *Supports,
    void **Resources
);

typedef uintn_t (EFIAPI *EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES)(
    EFI_PCI_IO_PROTOCOL *This,
    uint64_t Attributes,
    uint8_t BarIndex,
    uint64_t *Offset,
    uint64_t *Length
);

struct _EFI_PCI_IO_PROTOCOL {
    EFI_PCI_IO_PROTOCOL_POLL_IO_MEM           PollMem;
    EFI_PCI_IO_PROTOCOL_POLL_IO_MEM           PollIo;
    EFI_PCI_IO_PROTOCOL_ACCESS                Mem;
    EFI_PCI_IO_PROTOCOL_ACCESS                Io;
    EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS         Pci;
    EFI_PCI_IO_PROTOCOL_COPY_MEM              CopyMem;
    EFI_PCI_IO_PROTOCOL_MAP                   Map;
    EFI_PCI_IO_PROTOCOL_UNMAP                 Unmap;
    EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER       AllocateBuffer;
    EFI_PCI_IO_PROTOCOL_FREE_BUFFER           FreeBuffer;
    EFI_PCI_IO_PROTOCOL_FLUSH                 Flush;
    EFI_PCI_IO_PROTOCOL_GET_LOCATION          GetLocation;
    EFI_PCI_IO_PROTOCOL_ATTRIBUTES            Attributes;
    EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES    GetBarAttributes;
    EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES    SetBarAttributes;

    uint64_t RomSize;

    void    *RomImage;
};

/* pci */
typedef struct {
    uint16_t VendorId;
    uint16_t DeviceId;
    uint16_t Command;
    uint16_t Status;
    uint8_t RevisionID;
    uint8_t ClassCode[3];
    uint8_t CacheLineSize;
    uint8_t LatencyTimer;
    uint8_t HeaderType;
    uint8_t BIST;
} PCI_DEVICE_INDEPENDENT_REGION;

typedef struct {
    uint32_t Bar[6];
    uint32_t CISPtr;
    uint16_t SubsystemVendorID;
    uint16_t SubsystemID;
    uint32_t ExpansionRomBar;
    uint8_t CapabilityPtr;
    uint8_t Reserved1[3];
    uint32_t Reserved2;
    uint8_t InterruptLine;
    uint8_t InterruptPin;
    uint8_t MinGnt;
    uint8_t MaxLat;
} PCI_DEVICE_HEADER_TYPE_REGION;

typedef struct {
    PCI_DEVICE_INDEPENDENT_REGION Hdr;
    PCI_DEVICE_HEADER_TYPE_REGION Device;
} PCI_TYPE00;

typedef struct {
    uint8_t Desc;
    uint16_t Len;
    uint8_t ResType;
    uint8_t GenFlag;
    uint8_t SpecificFlag;
    uint64_t AddrSpaceGranularity;
    uint64_t AddrRangeMin;
    uint64_t AddrRangeMax;
    uint64_t AddrTranslationOffset;
    uint64_t AddrLen;
} EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR;

#pragma pack(pop)

