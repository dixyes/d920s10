
POSIX_UEFI_PATH = posix-uefi
UACPI_PATH = uACPI
CC = clang
HOST_CC = clang

PLATFORM_CFLAGS = \
	-target arm64-unknown-windows \
    -ffreestanding \
    -fshort-wchar \
    -mno-red-zone \
	-Wframe-larger-than=8192

OPTIMIZE_CFLAGS = \
    -O3 -ffast-math \

LINT_CFLAGS = \
    -Wall

INCLUDES = \
	-I. \
	-I$(POSIX_UEFI_PATH)/uefi \
	-I$(UACPI_PATH)/include

CFLAGS = $(PLATFORM_CFLAGS) $(OPTIMIZE_CFLAGS) $(LINT_CFLAGS) $(INCLUDES)

HOST_CFLAGS = \
	-Wall

PLATFORM_LDFLAGS = \
	-target arm64-unknown-windows \
	-nostdlib \
	'-Wl,-entry:uefi_init' \
	'-Wl,-subsystem:efi_application' \
	-fuse-ld=lld-link

LDFLAGS = $(PLATFORM_LDFLAGS) -Wl,-DEBUG

POSIX_UEFI_OBJS = \
	$(POSIX_UEFI_PATH)/uefi/crt_aarch64.o \
	$(POSIX_UEFI_PATH)/uefi/stdio.o \
	$(POSIX_UEFI_PATH)/uefi/stdlib.o \
	$(POSIX_UEFI_PATH)/uefi/time.o \
	$(POSIX_UEFI_PATH)/uefi/string.o

.PHONY: all
all: posix-uefi uacpi tablesfix.efi readnor.efi

.PHONY: posix-uefi
posix-uefi:
	if [ ! -d "${POSIX_UEFI_PATH}/uefi" ]; then \
		echo "Error: posix-uefi directory not found! Please clone submodules first." >&2; \
		echo "    git submodule update --init --recursive" >&2; \
		exit 1; \
	fi
	$(MAKE) $(POSIX_UEFI_OBJS)

$(POSIX_UEFI_PATH)/uefi/%.o: $(POSIX_UEFI_PATH)/uefi/%.c $(POSIX_UEFI_PATH)/uefi.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: posix-uefi-clean
posix-uefi-clean:
	rm -f $(POSIX_UEFI_OBJS)

UACPI_OBJS = \
	$(UACPI_PATH)/source/tables.o \
	$(UACPI_PATH)/source/uacpi.o \
	$(UACPI_PATH)/source/utilities.o \
	$(UACPI_PATH)/source/shareable.o \
	$(UACPI_PATH)/source/interpreter.o \
	$(UACPI_PATH)/source/opcodes.o \
	$(UACPI_PATH)/source/namespace.o \
	$(UACPI_PATH)/source/types.o \
	$(UACPI_PATH)/source/stdlib.o \
	$(UACPI_PATH)/source/opregion.o \
	$(UACPI_PATH)/source/default_handlers.o \
	$(UACPI_PATH)/source/osi.o \
	$(UACPI_PATH)/source/io.o \
	$(UACPI_PATH)/source/notify.o \
	$(UACPI_PATH)/source/sleep.o \
	$(UACPI_PATH)/source/registers.o \
	$(UACPI_PATH)/source/resources.o \
	$(UACPI_PATH)/source/mutex.o

UACPI_DEFS = \
	-DUACPI_REDUCED_HARDWARE \
	-DUACPI_KERNEL_INITIALIZATION \
	-DUACPI_NATIVE_ALLOC_ZEROED

.PHONY: uacpi
uacpi:
	if [ ! -d "${UACPI_PATH}" ]; then \
		echo "Error: uacpi directory not found! Please clone submodules first." >&2; \
		echo "    git submodule update --init --recursive" >&2; \
		exit 1; \
	fi
	$(MAKE) $(UACPI_OBJS)

$(UACPI_PATH)/source/%.o: $(UACPI_PATH)/source/%.c uacpi_platform.h
	$(CC) $(UACPI_DEFS) $(CFLAGS) -c $< -o $@

%.o: %.c definations.h
	$(CC) $(UACPI_DEFS) $(CFLAGS) -c $< -o $@

.PHONY: uacpi-clean
uacpi-clean:
	rm -f $(UACPI_OBJS)

tablesfix.efi: tablesfix.o dsdt_fix.o hob.o aml.o uacpi_kernel.o $(POSIX_UEFI_OBJS) $(UACPI_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

readnor.efi: readnor.o $(POSIX_UEFI_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

patch_bl: patch_bl.c
	$(HOST_CC) $(HOST_CFLAGS) -o $@ $<

.PHONY: clean
clean: posix-uefi-clean uacpi-clean
	rm -f *.o *.efi
