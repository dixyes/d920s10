DefinitionBlock ("", "SSDT", 2, "DIXYES", "SR358PCI", 0)
{
    // External (_SB.PCI0, DeviceObj)
    // External (_SB.PCI1, DeviceObj)
    // External (_SB.PCI2, DeviceObj)
    // External (_SB.PCI3, DeviceObj)
    // External (_SB.PCI4, DeviceObj)

    External (_SB.PCI0._PRT, PkgObj)
    External (_SB.PCI1._PRT, PkgObj)
    External (_SB.PCI2._PRT, PkgObj)
    External (_SB.PCI3._PRT, PkgObj)
    External (_SB.PCI4._PRT, PkgObj)

    External (_SB.LNKA, DeviceObj)
    External (_SB.LNKB, DeviceObj)
    External (_SB.LNKC, DeviceObj)
    External (_SB.LNKD, DeviceObj)

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

    // disable intx
    Scope (\_SB.LNKA)
    {
        Method (_STA, 0, NotSerialized) {
            Local0 = Package () { // _PRT: PCI Routing Table

                Package (0x04)
                {
                    0xFFFF, // Address
                    Zero, // Pin
                    Zero, // Source
                    0x33 // Source Index
                }, 
                Package (0x04)
                {
                    0xFFFF, // Address
                    One, // Pin
                    Zero, // Source
                    0x34 // Source Index
                }, 
                Package (0x04)
                {
                    0xFFFF, // Address
                    2, // Pin
                    Zero, // Source
                    0x35 // Source Index
                }, 
                Package (0x04)
                {
                    0xFFFF, // Address
                    3, // Pin
                    Zero, // Source
                    0x36 // Source Index
                }, 

                Package (0x04)
                {
                    0x1FFFF, // Address
                    Zero, // Pin
                    Zero, // Source
                    0x33 // Source Index
                }, 
                Package (0x04)
                {
                    0x1FFFF, // Address
                    One, // Pin
                    Zero, // Source
                    0x34 // Source Index
                }, 
                Package (0x04)
                {
                    0x1FFFF, // Address
                    2, // Pin
                    Zero, // Source
                    0x35 // Source Index
                }, 
                Package (0x04)
                {
                    0x1FFFF, // Address
                    3, // Pin
                    Zero, // Source
                    0x36 // Source Index
                }, 

                Package (0x04)
                {
                    0x2FFFF, // Address
                    Zero, // Pin
                    Zero, // Source
                    0x33 // Source Index
                }, 
                Package (0x04)
                {
                    0x2FFFF, // Address
                    One, // Pin
                    Zero, // Source
                    0x34 // Source Index
                }, 
                Package (0x04)
                {
                    0x2FFFF, // Address
                    2, // Pin
                    Zero, // Source
                    0x35 // Source Index
                }, 
                Package (0x04)
                {
                    0x2FFFF, // Address
                    3, // Pin
                    Zero, // Source
                    0x36 // Source Index
                }, 
            }
            CopyObject (Local0, \_SB.PCI0._PRT)
            CopyObject (Local0, \_SB.PCI1._PRT)
            CopyObject (Local0, \_SB.PCI2._PRT)
            CopyObject (Local0, \_SB.PCI3._PRT)
            CopyObject (Local0, \_SB.PCI4._PRT)

            Return (Zero)
        }
    }
    Scope (\_SB.LNKB)
    {
        Name (_STA, Zero)
    }
    Scope (\_SB.LNKC)
    {
        Name (_STA, Zero)
    }
    Scope (\_SB.LNKD)
    {
        Name (_STA, Zero)
    }

    // override PCI devices _PRT
    // Scope (\_SB.PCI0)
    // {
    //     Name (_PRT, Package () { // _PRT: PCI Routing Table
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             0, // Pin
    //             0, // Source
    //             0x33 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             1, // Pin
    //             0, // Source
    //             0x34 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             2, // Pin
    //             0, // Source
    //             0x35 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             3, // Pin
    //             0, // Source
    //             0x36 // Source Index
    //         }, 
    //     })
    // }
    // Scope (\_SB.PCI1)
    // {
    //     Name (_PRT, Package () { // _PRT: PCI Routing Table
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             0, // Pin
    //             0, // Source
    //             0x33 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             1, // Pin
    //             0, // Source
    //             0x34 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             2, // Pin
    //             0, // Source
    //             0x35 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             3, // Pin
    //             0, // Source
    //             0x36 // Source Index
    //         }, 
    //     })
    // }
    // Scope (\_SB.PCI2)
    // {
    //     Name (_PRT, Package () { // _PRT: PCI Routing Table
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             0, // Pin
    //             0, // Source
    //             0x33 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             1, // Pin
    //             0, // Source
    //             0x34 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             2, // Pin
    //             0, // Source
    //             0x35 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             3, // Pin
    //             0, // Source
    //             0x36 // Source Index
    //         }, 
    //     })
    // }
    // Scope (\_SB.PCI3)
    // {
    //     Name (_PRT, Package () { // _PRT: PCI Routing Table
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             0, // Pin
    //             0, // Source
    //             0x33 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             1, // Pin
    //             0, // Source
    //             0x34 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             2, // Pin
    //             0, // Source
    //             0x35 // Source Index
    //         }, 
    //         Package (0x04)
    //         {
    //             0xFFFF, // Address
    //             3, // Pin
    //             0, // Source
    //             0x36 // Source Index
    //         }, 
    //     })
    // }
    
}

