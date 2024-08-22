DefinitionBlock ("", "SSDT", 2, "DIXYES", "SR358PCI", 0)
{

    Scope (_SB)
    {
        // reserve ECAM memory range
        Device (RES0)
        {
            Name (_HID, EISAID ("PNP0C02"))
            Name (_CRS, ResourceTemplate () {
                QWordMemory (ResourceConsumer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0,         // Granularity
                    0x80040000000,  // Range Minimum
                    0x8004fffffff,  // Range Maximum
                    0,           // Translation Offset
                    0x10000000,  // Length
                    ,,)
            })
        }
    }
}

