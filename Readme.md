# ACPI fix for D920L11 Windows/Linux boot

Works with ~~D920S10/THTF TK620/W510~~ D920L11/THTF TK630/PT620K firmware, should also work for other 1-way hi1620 systems (~~D920L11~~ D920S10 maybe)

- Visit [tk630-win11-26h2](https://github.com/6DammK9/tk630-win11-26h2) and [tk630-win11-23h2](https://github.com/6DammK9/tk630-win11-23h2) for "PC build guide". The contents below are kept for reference.
  
- Breaking changes from original repo:
  - `patch_bl.c` is using different hex address
  - `Makefile` is using `clang` in Winodws WSL environment (amd64) for cross arch build target.
  - `tablesfix.c` has chain load disabled. Boot with `startup.nsh` instead (Post-install). For OS installation, use 2 USBs instead of 1, which this repo being bootloader, and the Windows installation media can be untouched.

- **Only tested for 26H2.** For 23H2, use my prebuilt `tablesfix-tk630-nochain.efi` in [tk630-win11-23h2](https://github.com/6DammK9/tk630-win11-23h2) or force W510 option in the older [20240709](https://github.com/dixyes/d920s10/releases/tag/20240709) version.
  - TK630 may not have UEFI Shell, somehow it switch to next boot option when the EFI application exit, so just rename `tablesfix-tk630-nochain.efi` into `bootaa64.efi` will be effective.
  - 24H1 onward requires materials more than this repo. Make sure check out my guide above.

------

## Usage

```
Shell> .\tablesfix.efi
```

or save it to EFI\BOOT\bootaa64.efi to chainload

## Build

```bash
git clone --recursive (this repo) d920s10
cd d920s10
make
```

## Details

0. Added DBG2 and SPCR for debug
1. PCCT table is corrupt, removed
2. RTC0 missing _HID, patched with ssdt
3. Offline DDRCs make linux hisi_pmu stuck (can be disabled by kernel param `initcall_blacklist=hisi_ddrc_pmu_module_init`), removed via ssdt
4. I have not found any gpu with win aa64 drivers, so use gop resolution changed it
5. When not invoked from cli, it chainloads bootmgr

## MPAM

arm8.2 adds MPAM (Memory Partitioning and Monitoring) support, Hi1620 supports MPAMv1.0, but BL31 does not initialize it, so Linux kernel (since 6.13) will fail to boot very early. The patch_bl tool can patch firmware to initialize MPAM, and then OS can boot normally.

**The patch is ABSOLUTELY NO WARRANTY,**

**USE AT YOUR OWN RISK**

```bash
# yet only for little-endian linux
make patch_bl
./patch_bl programmer_dump.bin patched.bin
# flash back patched.bin to SPI flash
```

## License

```text
ACPI fix for D920S10 boot
Copyright (C) 2023  Yun Dou <dixyes@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```
