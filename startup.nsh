# startup.nsh is in the USB root dir. bootaa64.efi is the UEFI shell "shellaa64.efi".
# %homefilesystem% will bind to the UEFI Shell.
%homefilesystem%\efi\boot\tablesfix.efi
# You will need this for 24H1 onward, but this repo stays at "23H2 untested"
# load -nc %homefilesystem%\efi\boot\undefshim_driver.efi

# Boot Windows EFI.
# FS0:\EFI\Boot\bootaa64.efi
for %b in 0 1 2 3 4 5 6 7 8 then
    if exist FS%b:\EFI\Microsoft\Boot\bootmgfw.efi then
        FS%b:\EFI\Microsoft\Boot\bootmgfw.efi
    endif
endfor