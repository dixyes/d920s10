/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20230628 (64-bit version)
 * Copyright (c) 2000 - 2023 Intel Corporation
 * 
 * Disassembling to non-symbolic legacy ASL operators
 *
 * Disassembly of ssdt0000.bin, Sat Jul 13 02:20:05 2024
 *
 * Original Table Header:
 *     Signature        "SSDT"
 *     Length           0x000011A9 (4521)
 *     Revision         0x02
 *     Checksum         0x75
 *     OEM ID           "ARMLTD"
 *     OEM Table ID     "PHYTIUM "
 *     OEM Revision     0x20180509 (538445065)
 *     Compiler ID      "INTL"
 *     Compiler Version 0x20191018 (538513432)
 */
DefinitionBlock ("", "SSDT", 2, "ARMLTD", "PHYTIUM ", 0x20180509)
{
    OperationRegion (GNVS, SystemMemory, 0xF2C70000, 0x00B8)
    Field (GNVS, AnyAcc, Lock, Preserve)
    {
        Offset (0x08), 
        R0IB,   32, 
        R1IB,   32, 
        R2IB,   32, 
        R3IB,   32, 
        R4IB,   32, 
        R0IL,   32, 
        R1IL,   32, 
        R2IL,   32, 
        R3IL,   32, 
        R4IL,   32, 
        RP0B,   32, 
        RP1B,   32, 
        RP2B,   32, 
        RP3B,   32, 
        RP4B,   32, 
        RP0L,   32, 
        RP1L,   32, 
        RP2L,   32, 
        RP3L,   32, 
        RP4L,   32, 
        R0HB,   64, 
        R1HB,   64, 
        R2HB,   64, 
        R3HB,   64, 
        R4HB,   64, 
        R0HL,   64, 
        R1HL,   64, 
        R2HL,   64, 
        R3HL,   64, 
        R4HL,   64, 
        RPCT,   8, 
        R0BB,   8, 
        R1BB,   8, 
        R2BB,   8, 
        R3BB,   8, 
        R4BB,   8, 
        R0BL,   8, 
        R1BL,   8, 
        R2BL,   8, 
        R3BL,   8, 
        R4BL,   8
    }

    Scope (_SB)
    {
        Device (LNKA)
        {
            Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)  // _HID: Hardware ID
            Name (_UID, One)  // _UID: Unique ID
            Name (_PRS, ResourceTemplate ()  // _PRS: Possible Resource Settings
            {
                Interrupt (ResourceProducer, Level, ActiveHigh, Exclusive, ,, )
                {
                    0x00000053,
                }
            })
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Return (_PRS) /* \_SB_.LNKA._PRS */
            }

            Method (_SRS, 1, NotSerialized)  // _SRS: Set Resource Settings
            {
            }

            Method (_DIS, 0, NotSerialized)  // _DIS: Disable Device
            {
            }
        }

        Device (LNKB)
        {
            Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)  // _HID: Hardware ID
            Name (_UID, 0x02)  // _UID: Unique ID
            Name (_PRS, ResourceTemplate ()  // _PRS: Possible Resource Settings
            {
                Interrupt (ResourceProducer, Level, ActiveHigh, Exclusive, ,, )
                {
                    0x00000054,
                }
            })
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Return (_PRS) /* \_SB_.LNKB._PRS */
            }

            Method (_SRS, 1, NotSerialized)  // _SRS: Set Resource Settings
            {
            }

            Method (_DIS, 0, NotSerialized)  // _DIS: Disable Device
            {
            }
        }

        Device (LNKC)
        {
            Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)  // _HID: Hardware ID
            Name (_UID, 0x03)  // _UID: Unique ID
            Name (_PRS, ResourceTemplate ()  // _PRS: Possible Resource Settings
            {
                Interrupt (ResourceProducer, Level, ActiveHigh, Exclusive, ,, )
                {
                    0x00000055,
                }
            })
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Return (_PRS) /* \_SB_.LNKC._PRS */
            }

            Method (_SRS, 1, NotSerialized)  // _SRS: Set Resource Settings
            {
            }

            Method (_DIS, 0, NotSerialized)  // _DIS: Disable Device
            {
            }
        }

        Device (LNKD)
        {
            Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)  // _HID: Hardware ID
            Name (_UID, 0x04)  // _UID: Unique ID
            Name (_PRS, ResourceTemplate ()  // _PRS: Possible Resource Settings
            {
                Interrupt (ResourceProducer, Level, ActiveHigh, Exclusive, ,, )
                {
                    0x00000056,
                }
            })
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Return (_PRS) /* \_SB_.LNKD._PRS */
            }

            Method (_SRS, 1, NotSerialized)  // _SRS: Set Resource Settings
            {
            }

            Method (_DIS, 0, NotSerialized)  // _DIS: Disable Device
            {
            }
        }

        Device (PCI0)
        {
            Name (_HID, EisaId ("PNP0A08") /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, EisaId ("PNP0A03") /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, Zero)  // _SEG: PCI Segment
            Name (_CCA, One)  // _CCA: Cache Coherency Attribute
            Method (_BBN, 0, NotSerialized)  // _BBN: BIOS Bus Number
            {
                Return (\R0BB)
            }

            Device (RP0)
            {
                Name (_ADR, Zero)  // _ADR: Address
            }

            Name (_PRT, Package (0x04)  // _PRT: PCI Routing Table
            {
                Package (0x04)
                {
                    0xFFFF, 
                    Zero, 
                    LNKA, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    One, 
                    LNKB, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x02, 
                    LNKC, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x03, 
                    LNKD, 
                    Zero
                }
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                Return (0x0F)
            }

            Name (CRS0, ResourceTemplate ()
            {
                WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                    0x0000,             // Granularity
                    0x0000,             // Range Minimum
                    0x0000,             // Range Maximum
                    0x0000,             // Translation Offset
                    0x0000,             // Length
                    ,, _Y00)
                QWordIO (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080050000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y01, TypeTranslation, DenseTranslation)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y02, AddressRangeMemory, TypeStatic)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y03, AddressRangeMemory, TypeStatic)
            })
            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                CreateWordField (CRS0, \_SB.PCI0._Y00._MIN, MIN0)  // _MIN: Minimum Base Address
                CreateWordField (CRS0, \_SB.PCI0._Y00._MAX, MAX0)  // _MAX: Maximum Base Address
                CreateWordField (CRS0, \_SB.PCI0._Y00._LEN, LEN0)  // _LEN: Length
                Store (\R0BB, MIN0) /* \_SB_.PCI0._CRS.MIN0 */
                Store (\R0BL, MAX0) /* \_SB_.PCI0._CRS.MAX0 */
                Subtract (MAX0, MIN0, Local0)
                Add (Local0, One, LEN0) /* \_SB_.PCI0._CRS.LEN0 */
                CreateQWordField (CRS0, \_SB.PCI0._Y01._MIN, MIN1)  // _MIN: Minimum Base Address
                CreateQWordField (CRS0, \_SB.PCI0._Y01._MAX, MAX1)  // _MAX: Maximum Base Address
                CreateQWordField (CRS0, \_SB.PCI0._Y01._LEN, LEN1)  // _LEN: Length
                Store (\R0IB, MIN1) /* \_SB_.PCI0._CRS.MIN1 */
                Store (\R0IL, LEN1) /* \_SB_.PCI0._CRS.LEN1 */
                Store (LEN1, Local0)
                Add (MIN1, Decrement (Local0), MAX1) /* \_SB_.PCI0._CRS.MAX1 */
                CreateQWordField (CRS0, \_SB.PCI0._Y02._MIN, MIN2)  // _MIN: Minimum Base Address
                CreateQWordField (CRS0, \_SB.PCI0._Y02._MAX, MAX2)  // _MAX: Maximum Base Address
                CreateQWordField (CRS0, \_SB.PCI0._Y02._LEN, LEN2)  // _LEN: Length
                Store (\RP0B, MIN2) /* \_SB_.PCI0._CRS.MIN2 */
                Store (\RP0L, LEN2) /* \_SB_.PCI0._CRS.LEN2 */
                Store (LEN2, Local0)
                Add (MIN2, Decrement (Local0), MAX2) /* \_SB_.PCI0._CRS.MAX2 */
                CreateQWordField (CRS0, \_SB.PCI0._Y03._MIN, MIN3)  // _MIN: Minimum Base Address
                CreateQWordField (CRS0, \_SB.PCI0._Y03._MAX, MAX3)  // _MAX: Maximum Base Address
                CreateQWordField (CRS0, \_SB.PCI0._Y03._LEN, LEN3)  // _LEN: Length
                Store (\R0HB, MIN3) /* \_SB_.PCI0._CRS.MIN3 */
                Store (\R0HL, LEN3) /* \_SB_.PCI0._CRS.LEN3 */
                Store (LEN3, Local0)
                Add (MIN3, Decrement (Local0), MAX3) /* \_SB_.PCI0._CRS.MAX3 */
                Return (CRS0) /* \_SB_.PCI0.CRS0 */
            }

            Name (SUPP, Zero)
            Name (CTRL, Zero)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, Zero, CDW1)
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI0.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI0.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI0.CTRL */
                    }

                    And (CTRL, 0x10, CTRL) /* \_SB_.PCI0.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI0._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI0._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI0._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI0._OSC.CDW1 */
                    Return (Arg3)
                }
            }
        }

        Device (PCI1)
        {
            Name (_HID, EisaId ("PNP0A08") /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, EisaId ("PNP0A03") /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, Zero)  // _SEG: PCI Segment
            Name (_CCA, One)  // _CCA: Cache Coherency Attribute
            Method (_BBN, 0, NotSerialized)  // _BBN: BIOS Bus Number
            {
                Return (\R1BB)
            }

            Device (RP0)
            {
                Name (_ADR, Zero)  // _ADR: Address
            }

            Name (_PRT, Package (0x04)  // _PRT: PCI Routing Table
            {
                Package (0x04)
                {
                    0xFFFF, 
                    Zero, 
                    LNKA, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    One, 
                    LNKB, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x02, 
                    LNKC, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x03, 
                    LNKD, 
                    Zero
                }
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (LGreater (\RPCT, One))
                {
                    Return (0x0F)
                }

                Return (Zero)
            }

            Name (CRS1, ResourceTemplate ()
            {
                WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                    0x0000,             // Granularity
                    0x0000,             // Range Minimum
                    0x0000,             // Range Maximum
                    0x0000,             // Translation Offset
                    0x0000,             // Length
                    ,, _Y04)
                QWordIO (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080050000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y05, TypeTranslation, DenseTranslation)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y06, AddressRangeMemory, TypeStatic)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y07, AddressRangeMemory, TypeStatic)
            })
            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                CreateWordField (CRS1, \_SB.PCI1._Y04._MIN, MIN0)  // _MIN: Minimum Base Address
                CreateWordField (CRS1, \_SB.PCI1._Y04._MAX, MAX0)  // _MAX: Maximum Base Address
                CreateWordField (CRS1, \_SB.PCI1._Y04._LEN, LEN0)  // _LEN: Length
                Store (\R1BB, MIN0) /* \_SB_.PCI1._CRS.MIN0 */
                Store (\R1BL, MAX0) /* \_SB_.PCI1._CRS.MAX0 */
                Subtract (MAX0, MIN0, Local0)
                Add (Local0, One, LEN0) /* \_SB_.PCI1._CRS.LEN0 */
                CreateQWordField (CRS1, \_SB.PCI1._Y05._MIN, MIN1)  // _MIN: Minimum Base Address
                CreateQWordField (CRS1, \_SB.PCI1._Y05._MAX, MAX1)  // _MAX: Maximum Base Address
                CreateQWordField (CRS1, \_SB.PCI1._Y05._LEN, LEN1)  // _LEN: Length
                Store (\R1IB, MIN1) /* \_SB_.PCI1._CRS.MIN1 */
                Store (\R1IL, LEN1) /* \_SB_.PCI1._CRS.LEN1 */
                Store (LEN1, Local0)
                Add (MIN1, Decrement (Local0), MAX1) /* \_SB_.PCI1._CRS.MAX1 */
                CreateQWordField (CRS1, \_SB.PCI1._Y06._MIN, MIN2)  // _MIN: Minimum Base Address
                CreateQWordField (CRS1, \_SB.PCI1._Y06._MAX, MAX2)  // _MAX: Maximum Base Address
                CreateQWordField (CRS1, \_SB.PCI1._Y06._LEN, LEN2)  // _LEN: Length
                Store (\RP1B, MIN2) /* \_SB_.PCI1._CRS.MIN2 */
                Store (\RP1L, LEN2) /* \_SB_.PCI1._CRS.LEN2 */
                Store (LEN2, Local0)
                Add (MIN2, Decrement (Local0), MAX2) /* \_SB_.PCI1._CRS.MAX2 */
                CreateQWordField (CRS1, \_SB.PCI1._Y07._MIN, MIN3)  // _MIN: Minimum Base Address
                CreateQWordField (CRS1, \_SB.PCI1._Y07._MAX, MAX3)  // _MAX: Maximum Base Address
                CreateQWordField (CRS1, \_SB.PCI1._Y07._LEN, LEN3)  // _LEN: Length
                Store (\R1HB, MIN3) /* \_SB_.PCI1._CRS.MIN3 */
                Store (\R1HL, LEN3) /* \_SB_.PCI1._CRS.LEN3 */
                Store (LEN3, Local0)
                Add (MIN3, Decrement (Local0), MAX3) /* \_SB_.PCI1._CRS.MAX3 */
                Return (CRS1) /* \_SB_.PCI1.CRS1 */
            }

            Name (SUPP, Zero)
            Name (CTRL, Zero)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, Zero, CDW1)
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI1.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI1.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI1.CTRL */
                    }

                    And (CTRL, 0x10, CTRL) /* \_SB_.PCI1.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI1._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI1._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI1._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI1._OSC.CDW1 */
                    Return (Arg3)
                }
            }
        }

        Device (PCI2)
        {
            Name (_HID, EisaId ("PNP0A08") /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, EisaId ("PNP0A03") /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, Zero)  // _SEG: PCI Segment
            Name (_CCA, One)  // _CCA: Cache Coherency Attribute
            Method (_BBN, 0, NotSerialized)  // _BBN: BIOS Bus Number
            {
                Return (\R2BB)
            }

            Device (RP0)
            {
                Name (_ADR, Zero)  // _ADR: Address
            }

            Name (_PRT, Package (0x04)  // _PRT: PCI Routing Table
            {
                Package (0x04)
                {
                    0xFFFF, 
                    Zero, 
                    LNKA, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    One, 
                    LNKB, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x02, 
                    LNKC, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x03, 
                    LNKD, 
                    Zero
                }
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (LGreater (\RPCT, 0x02))
                {
                    Return (0x0F)
                }

                Return (Zero)
            }

            Name (CRS2, ResourceTemplate ()
            {
                WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                    0x0000,             // Granularity
                    0x0000,             // Range Minimum
                    0x0000,             // Range Maximum
                    0x0000,             // Translation Offset
                    0x0000,             // Length
                    ,, _Y08)
                QWordIO (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080050000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y09, TypeTranslation, DenseTranslation)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y0A, AddressRangeMemory, TypeStatic)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y0B, AddressRangeMemory, TypeStatic)
            })
            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                CreateWordField (CRS2, \_SB.PCI2._Y08._MIN, MIN0)  // _MIN: Minimum Base Address
                CreateWordField (CRS2, \_SB.PCI2._Y08._MAX, MAX0)  // _MAX: Maximum Base Address
                CreateWordField (CRS2, \_SB.PCI2._Y08._LEN, LEN0)  // _LEN: Length
                Store (\R2BB, MIN0) /* \_SB_.PCI2._CRS.MIN0 */
                Store (\R2BL, MAX0) /* \_SB_.PCI2._CRS.MAX0 */
                Subtract (MAX0, MIN0, Local0)
                Add (Local0, One, LEN0) /* \_SB_.PCI2._CRS.LEN0 */
                CreateQWordField (CRS2, \_SB.PCI2._Y09._MIN, MIN1)  // _MIN: Minimum Base Address
                CreateQWordField (CRS2, \_SB.PCI2._Y09._MAX, MAX1)  // _MAX: Maximum Base Address
                CreateQWordField (CRS2, \_SB.PCI2._Y09._LEN, LEN1)  // _LEN: Length
                Store (\R2IB, MIN1) /* \_SB_.PCI2._CRS.MIN1 */
                Store (\R2IL, LEN1) /* \_SB_.PCI2._CRS.LEN1 */
                Store (LEN1, Local0)
                Add (MIN1, Decrement (Local0), MAX1) /* \_SB_.PCI2._CRS.MAX1 */
                CreateQWordField (CRS2, \_SB.PCI2._Y0A._MIN, MIN2)  // _MIN: Minimum Base Address
                CreateQWordField (CRS2, \_SB.PCI2._Y0A._MAX, MAX2)  // _MAX: Maximum Base Address
                CreateQWordField (CRS2, \_SB.PCI2._Y0A._LEN, LEN2)  // _LEN: Length
                Store (\RP2B, MIN2) /* \_SB_.PCI2._CRS.MIN2 */
                Store (\RP2L, LEN2) /* \_SB_.PCI2._CRS.LEN2 */
                Store (LEN2, Local0)
                Add (MIN2, Decrement (Local0), MAX2) /* \_SB_.PCI2._CRS.MAX2 */
                CreateQWordField (CRS2, \_SB.PCI2._Y0B._MIN, MIN3)  // _MIN: Minimum Base Address
                CreateQWordField (CRS2, \_SB.PCI2._Y0B._MAX, MAX3)  // _MAX: Maximum Base Address
                CreateQWordField (CRS2, \_SB.PCI2._Y0B._LEN, LEN3)  // _LEN: Length
                Store (\R2HB, MIN3) /* \_SB_.PCI2._CRS.MIN3 */
                Store (\R2HL, LEN3) /* \_SB_.PCI2._CRS.LEN3 */
                Store (LEN3, Local0)
                Add (MIN3, Decrement (Local0), MAX3) /* \_SB_.PCI2._CRS.MAX3 */
                Return (CRS2) /* \_SB_.PCI2.CRS2 */
            }

            Name (SUPP, Zero)
            Name (CTRL, Zero)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, Zero, CDW1)
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI2.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI2.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI2.CTRL */
                    }

                    And (CTRL, 0x10, CTRL) /* \_SB_.PCI2.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI2._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI2._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI2._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI2._OSC.CDW1 */
                    Return (Arg3)
                }
            }
        }

        Device (PCI3)
        {
            Name (_HID, EisaId ("PNP0A08") /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, EisaId ("PNP0A03") /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, Zero)  // _SEG: PCI Segment
            Name (_CCA, One)  // _CCA: Cache Coherency Attribute
            Method (_BBN, 0, NotSerialized)  // _BBN: BIOS Bus Number
            {
                Return (\R3BB)
            }

            Device (RP0)
            {
                Name (_ADR, Zero)  // _ADR: Address
            }

            Name (_PRT, Package (0x04)  // _PRT: PCI Routing Table
            {
                Package (0x04)
                {
                    0xFFFF, 
                    Zero, 
                    LNKA, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    One, 
                    LNKB, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x02, 
                    LNKC, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x03, 
                    LNKD, 
                    Zero
                }
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (LGreater (\RPCT, 0x03))
                {
                    Return (0x0F)
                }

                Return (Zero)
            }

            Name (CRS3, ResourceTemplate ()
            {
                WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                    0x0000,             // Granularity
                    0x0000,             // Range Minimum
                    0x0000,             // Range Maximum
                    0x0000,             // Translation Offset
                    0x0000,             // Length
                    ,, _Y0C)
                QWordIO (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080050000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y0D, TypeTranslation, DenseTranslation)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y0E, AddressRangeMemory, TypeStatic)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y0F, AddressRangeMemory, TypeStatic)
            })
            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                CreateWordField (CRS3, \_SB.PCI3._Y0C._MIN, MIN0)  // _MIN: Minimum Base Address
                CreateWordField (CRS3, \_SB.PCI3._Y0C._MAX, MAX0)  // _MAX: Maximum Base Address
                CreateWordField (CRS3, \_SB.PCI3._Y0C._LEN, LEN0)  // _LEN: Length
                Store (\R3BB, MIN0) /* \_SB_.PCI3._CRS.MIN0 */
                Store (\R3BL, MAX0) /* \_SB_.PCI3._CRS.MAX0 */
                Subtract (MAX0, MIN0, Local0)
                Add (Local0, One, LEN0) /* \_SB_.PCI3._CRS.LEN0 */
                CreateQWordField (CRS3, \_SB.PCI3._Y0D._MIN, MIN1)  // _MIN: Minimum Base Address
                CreateQWordField (CRS3, \_SB.PCI3._Y0D._MAX, MAX1)  // _MAX: Maximum Base Address
                CreateQWordField (CRS3, \_SB.PCI3._Y0D._LEN, LEN1)  // _LEN: Length
                Store (\R3IB, MIN1) /* \_SB_.PCI3._CRS.MIN1 */
                Store (\R3IL, LEN1) /* \_SB_.PCI3._CRS.LEN1 */
                Store (LEN1, Local0)
                Add (MIN1, Decrement (Local0), MAX1) /* \_SB_.PCI3._CRS.MAX1 */
                CreateQWordField (CRS3, \_SB.PCI3._Y0E._MIN, MIN2)  // _MIN: Minimum Base Address
                CreateQWordField (CRS3, \_SB.PCI3._Y0E._MAX, MAX2)  // _MAX: Maximum Base Address
                CreateQWordField (CRS3, \_SB.PCI3._Y0E._LEN, LEN2)  // _LEN: Length
                Store (\RP3B, MIN2) /* \_SB_.PCI3._CRS.MIN2 */
                Store (\RP3L, LEN2) /* \_SB_.PCI3._CRS.LEN2 */
                Store (LEN2, Local0)
                Add (MIN2, Decrement (Local0), MAX2) /* \_SB_.PCI3._CRS.MAX2 */
                CreateQWordField (CRS3, \_SB.PCI3._Y0F._MIN, MIN3)  // _MIN: Minimum Base Address
                CreateQWordField (CRS3, \_SB.PCI3._Y0F._MAX, MAX3)  // _MAX: Maximum Base Address
                CreateQWordField (CRS3, \_SB.PCI3._Y0F._LEN, LEN3)  // _LEN: Length
                Store (\R3HB, MIN3) /* \_SB_.PCI3._CRS.MIN3 */
                Store (\R3HL, LEN3) /* \_SB_.PCI3._CRS.LEN3 */
                Store (LEN3, Local0)
                Add (MIN3, Decrement (Local0), MAX3) /* \_SB_.PCI3._CRS.MAX3 */
                Return (CRS3) /* \_SB_.PCI3.CRS3 */
            }

            Name (SUPP, Zero)
            Name (CTRL, Zero)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, Zero, CDW1)
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI3.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI3.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI3.CTRL */
                    }

                    And (CTRL, 0x10, CTRL) /* \_SB_.PCI3.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI3._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI3._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI3._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI3._OSC.CDW1 */
                    Return (Arg3)
                }
            }
        }

        Device (PCI4)
        {
            Name (_HID, EisaId ("PNP0A08") /* PCI Express Bus */)  // _HID: Hardware ID
            Name (_CID, EisaId ("PNP0A03") /* PCI Bus */)  // _CID: Compatible ID
            Name (_SEG, Zero)  // _SEG: PCI Segment
            Name (_CCA, One)  // _CCA: Cache Coherency Attribute
            Method (_BBN, 0, NotSerialized)  // _BBN: BIOS Bus Number
            {
                Return (\R4BB)
            }

            Device (RP0)
            {
                Name (_ADR, Zero)  // _ADR: Address
            }

            Name (_PRT, Package (0x04)  // _PRT: PCI Routing Table
            {
                Package (0x04)
                {
                    0xFFFF, 
                    Zero, 
                    LNKA, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    One, 
                    LNKB, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x02, 
                    LNKC, 
                    Zero
                }, 

                Package (0x04)
                {
                    0xFFFF, 
                    0x03, 
                    LNKD, 
                    Zero
                }
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (LGreater (\RPCT, 0x04))
                {
                    Return (0x0F)
                }

                Return (Zero)
            }

            Name (CRS4, ResourceTemplate ()
            {
                WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                    0x0000,             // Granularity
                    0x0000,             // Range Minimum
                    0x0000,             // Range Maximum
                    0x0000,             // Translation Offset
                    0x0000,             // Length
                    ,, _Y10)
                QWordIO (ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080050000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y11, TypeTranslation, DenseTranslation)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y12, AddressRangeMemory, TypeStatic)
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                    0x0000000000000000, // Granularity
                    0x0000000000000000, // Range Minimum
                    0x0000000000000000, // Range Maximum
                    0x0000080000000000, // Translation Offset
                    0x0000000000000000, // Length
                    ,, _Y13, AddressRangeMemory, TypeStatic)
            })
            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                CreateWordField (CRS4, \_SB.PCI4._Y10._MIN, MIN0)  // _MIN: Minimum Base Address
                CreateWordField (CRS4, \_SB.PCI4._Y10._MAX, MAX0)  // _MAX: Maximum Base Address
                CreateWordField (CRS4, \_SB.PCI4._Y10._LEN, LEN0)  // _LEN: Length
                Store (\R4BB, MIN0) /* \_SB_.PCI4._CRS.MIN0 */
                Store (\R4BL, MAX0) /* \_SB_.PCI4._CRS.MAX0 */
                Subtract (MAX0, MIN0, Local0)
                Add (Local0, One, LEN0) /* \_SB_.PCI4._CRS.LEN0 */
                CreateQWordField (CRS4, \_SB.PCI4._Y11._MIN, MIN1)  // _MIN: Minimum Base Address
                CreateQWordField (CRS4, \_SB.PCI4._Y11._MAX, MAX1)  // _MAX: Maximum Base Address
                CreateQWordField (CRS4, \_SB.PCI4._Y11._LEN, LEN1)  // _LEN: Length
                Store (\R4IB, MIN1) /* \_SB_.PCI4._CRS.MIN1 */
                Store (\R4IL, LEN1) /* \_SB_.PCI4._CRS.LEN1 */
                Store (LEN1, Local0)
                Add (MIN1, Decrement (Local0), MAX1) /* \_SB_.PCI4._CRS.MAX1 */
                CreateQWordField (CRS4, \_SB.PCI4._Y12._MIN, MIN2)  // _MIN: Minimum Base Address
                CreateQWordField (CRS4, \_SB.PCI4._Y12._MAX, MAX2)  // _MAX: Maximum Base Address
                CreateQWordField (CRS4, \_SB.PCI4._Y12._LEN, LEN2)  // _LEN: Length
                Store (\RP4B, MIN2) /* \_SB_.PCI4._CRS.MIN2 */
                Store (\RP4L, LEN2) /* \_SB_.PCI4._CRS.LEN2 */
                Store (LEN2, Local0)
                Add (MIN2, Decrement (Local0), MAX2) /* \_SB_.PCI4._CRS.MAX2 */
                CreateQWordField (CRS4, \_SB.PCI4._Y13._MIN, MIN3)  // _MIN: Minimum Base Address
                CreateQWordField (CRS4, \_SB.PCI4._Y13._MAX, MAX3)  // _MAX: Maximum Base Address
                CreateQWordField (CRS4, \_SB.PCI4._Y13._LEN, LEN3)  // _LEN: Length
                Store (\R4HB, MIN3) /* \_SB_.PCI4._CRS.MIN3 */
                Store (\R4HL, LEN3) /* \_SB_.PCI4._CRS.LEN3 */
                Store (LEN3, Local0)
                Add (MIN3, Decrement (Local0), MAX3) /* \_SB_.PCI4._CRS.MAX3 */
                Return (CRS4) /* \_SB_.PCI4.CRS4 */
            }

            Name (SUPP, Zero)
            Name (CTRL, Zero)
            Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                If (LEqual (Arg0, ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    CreateDWordField (Arg3, Zero, CDW1)
                    CreateDWordField (Arg3, 0x04, CDW2)
                    CreateDWordField (Arg3, 0x08, CDW3)
                    Store (CDW2, SUPP) /* \_SB_.PCI4.SUPP */
                    Store (CDW3, CTRL) /* \_SB_.PCI4.CTRL */
                    If (LNotEqual (And (SUPP, 0x16), 0x16))
                    {
                        And (CTRL, 0x1E, CTRL) /* \_SB_.PCI4.CTRL */
                    }

                    And (CTRL, 0x10, CTRL) /* \_SB_.PCI4.CTRL */
                    If (LNotEqual (Arg1, One))
                    {
                        Or (CDW1, 0x08, CDW1) /* \_SB_.PCI4._OSC.CDW1 */
                    }

                    If (LNotEqual (CDW3, CTRL))
                    {
                        Or (CDW1, 0x10, CDW1) /* \_SB_.PCI4._OSC.CDW1 */
                    }

                    Store (CTRL, CDW3) /* \_SB_.PCI4._OSC.CDW3 */
                    Return (Arg3)
                }
                Else
                {
                    Or (CDW1, 0x04, CDW1) /* \_SB_.PCI4._OSC.CDW1 */
                    Return (Arg3)
                }
            }
        }
    }
}

