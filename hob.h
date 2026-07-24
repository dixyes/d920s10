#ifndef _HOB_H_
#define _HOB_H_

#include "definations.h"

// hisi Memory Map HOB things

// from edk2-platforms/Silicon/Hisilicon/Include/Guid/MemoryMapData.h
// removed recently, see 202502-before-platform-removals tag
#define EFI_MEMORY_MAP_GUID \
  { \
    0xf8870015,0x6994,0x4b98,0x95,0xa2,0xbd,0x56,0xda,0x91,0xc0,0x7f \
  }

// from edk2-platforms/Silicon/Hisilicon/Hi1610/Include/PlatformArch.h
// removed recently, see 202502-before-platform-removals tag
#define MAX_SOCKET      2
#define MAX_CHANNEL     4
#define MAX_DIMM        3
#define MAX_RANK_DIMM   4

// from edk2-platforms/Silicon/Hisilicon/Include/Library/HwMemInitLib.h
// removed recently, see 202502-before-platform-removals tag
#define SPD_MODULE_PART         18
#define SPD_MODULE_PART_DDR4    20

struct DDR_RANK_DATA {
    uint8_t           Enabled;
};

typedef struct _DDR_DIMM_DATA {
    uint8_t           Enabled;
    uint8_t           DramType;           //Byte 2
    uint8_t           ModuleType;         //Byte 3
    uint8_t           BankNum;            //Byte 4
    uint8_t           RowBits;            //Byte 5
    uint8_t           ColBits;            //Byte 5
    uint8_t           SpdVdd;             //Byte 6
    uint8_t           RankNum;            //Byte 7
    uint8_t           PrimaryBusWidth;    //Byte 8
    uint8_t           ExtensionBusWidth;  //Byte 8
    uint8_t           SpdModPart[SPD_MODULE_PART];                // Module Part Number
    uint8_t           SpdModPartDDR4[SPD_MODULE_PART_DDR4];       // Module Part Number DDR4
    uint16_t          SpdMMfgId;              // Module Mfg Id from SPD
    uint32_t          SpdSerialNum;
    uint32_t          RankSize;
    uint16_t          DimmSize;
    uint16_t          DimmSpeed;
    uint16_t          SpdMMDate;
    struct DDR_RANK_DATA    Rank[MAX_RANK_DIMM];
} DDR_DIMM_DATA;

typedef struct _DDR_CHANNEL_DATA {
    uint8_t         Enabled;
    DDR_DIMM_DATA   Dimm[MAX_DIMM];
    uint8_t           CurrentDimmNum;
} DDR_CHANNEL_DATA;

typedef struct _GBL_DATA_INTERFACE {
    DDR_CHANNEL_DATA        Channel[MAX_SOCKET][MAX_CHANNEL];
    uint32_t                  DdrFreqIdx;
    uint32_t                  Freq;
    uint32_t                  EccEn;
    uint32_t                  MemSize;
    uint8_t                 SetupExist;
    // not care
    /*
    NVRAM_DATA              NvRamData;
    MEMORY_DATA             MemData;
    NUMAINFO_DATA           NumaInfo[MAX_SOCKET][MAX_NUM_PER_TYPE];
    */
} GBL_INTERFACE;

int hob_get_online_ddrcs (EFI_ACPI_TABLE_PROTOCOL *acpi_table, uintptr_t *online_ddrcs_base);

#endif // _HOB_H_
