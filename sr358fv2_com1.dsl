
DefinitionBlock ("", "SSDT", 2, "DIXYES", "SR358COM", 0)
{
    Scope (\_SB)
    {
        Device (COM1)
        {
            Name (_HID, "HISI0031")  // _HID: Hardware ID
            Name (_UID, One)  // _UID: Unique ID
            Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                QWordMemory (
                    ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
                    0, // Granularity
                    0x0000080028000000, // Range Minimum
                    0x0000080028000fff, // Range Maximum
                    0x0000000000000000, // Translation Offset
                    0x0000000000001000, // Length
                ,, , AddressRangeMemory, TypeStatic)
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                {
                    0x00000042,
                }
            })

            Name (_DSD, Package (0x02)  // _DSD: Device-Specific Data
            {
                ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301") /* Device Properties for _DSD */, 
                Package (0x04)
                {
                    Package (0x02)
                    {
                        "clock-frequency", 
                        0x02FAF080
                    }, 

                    Package (0x02)
                    {
                        "current-speed", 
                        115200
                    }, 

                    Package (0x02)
                    {
                        "reg-io-width", 
                        0x04
                    }, 

                    Package (0x02)
                    {
                        "reg-shift", 
                        0x02
                    }
                }
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                Return (0x0F)
            }
        }
    }
}