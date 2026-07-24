
DefinitionBlock ("", "SSDT", 2, "DIXYES", "REMDDRC ", 0)
{
    External (_SB.DDR0, DeviceObj)
    External (_SB.DDR0._CRS, UnknownObj)

    Scope (\_SB.DXY0)
    {
        Name (DOL0, Package () { /* 'D'DRC 'O'nline 'L'ist */
            0x94D20000, 0x94D30000, 0x94D40000, 0x94D50000,
            0x9CD30000, 0x9CD40000, 0x9CD50000, 0x9CD60000
        })

        Method (ISDO, 1, NotSerialized) /* IS 'D'DRC 'O'nline */
        {
            CreateQWordField (Arg0, 0x0E, BAS0)
            If (LNotEqual (Match (DOL0, MEQ, BAS0, MTR, Zero, Zero), Ones))
            {
                Return (0x0F)
            }
            Return (Zero)
        }
    }


    If (CondRefOf (\_SB.DDR0))
    {
        Scope (\_SB.DDR0)
        {
            Method (_STA, 0, NotSerialized)
            {
                If (CondRefOf (_CRS))
                {
                    Store (_CRS, Local0)
                    Return (\_SB.DXY0.ISDO (Local0))
                }
                Return (Zero)
            }
        }
    }
}
