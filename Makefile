
POSIX_UEFI_PATH = posix-uefi
CC = clang

PLATFORM_CFLAGS = \
	-target arm64-unknown-windows \
    -ffreestanding \
    -fshort-wchar \
    -mno-red-zone

OPTIMIZE_CFLAGS = \
    -O3 -ffast-math \

LINT_CFLAGS = \
    -Wall

INCLUDES = \
	-I${POSIX_UEFI_PATH}/uefi

CFLAGS = $(PLATFORM_CFLAGS) $(OPTIMIZE_CFLAGS) $(LINT_CFLAGS) $(INCLUDES)

PLATFORM_LDFLAGS = \
	-target arm64-unknown-windows \
	-nostdlib \
	'-Wl,-entry:uefi_init' \
	'-Wl,-subsystem:efi_application' \
	-fuse-ld=lld-link

LDFLAGS = $(PLATFORM_LDFLAGS)

POSIX_UEFI_OBJS = \
	posix-uefi/uefi/crt_aarch64.o \
	posix-uefi/uefi/stdio.o \
	posix-uefi/uefi/stdlib.o \
	posix-uefi/uefi/string.o \
	posix-uefi/uefi/time.o

.PHONY: all
all: posix-uefi tablesfix.efi readnor.efi

.PHONY: posix-uefi

posix-uefi:
	@echo "Building posix-uefi..."
	if [ ! -d "${POSIX_UEFI_PATH}/uefi" ]; then \
		echo "Error: posix-uefi directory not found! Please clone submodules first." >&2; \
		echo "    git submodule update --init --recursive" >&2; \
		exit 1; \
	fi
	$(MAKE) $(POSIX_UEFI_OBJS)

posix-uefi/uefi/%.o: $(POSIX_UEFI_PATH)/uefi/%.c $(POSIX_UEFI_PATH)/uefi.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c definations.h $(POSIX_UEFI_PATH)/uefi.h
	$(CC) $(CFLAGS) -c $< -o $@

tablesfix.efi: tablesfix.o $(POSIX_UEFI_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

readnor.efi: readnor.o $(POSIX_UEFI_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
