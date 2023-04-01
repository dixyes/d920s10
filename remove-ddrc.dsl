
DefinitionBlock ("", "SSDT", 2, "DIXYES", "DIXYES01", 0)
{
    External (_SB.DDR2, DeviceObj)
    External (_SB.DDR3, DeviceObj)
    
    Scope (\_SB.DDR2)
    {
        Name (_STA, Zero)
    }

    Scope (\_SB.DDR3)
    {
        Name (_STA, Zero)
    }
}
