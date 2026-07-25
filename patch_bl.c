
#ifndef IN_UEFI
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/sendfile.h>

#define yprintf(fmt, ...) fprintf(stderr, "\033[1;36m" fmt "\033[0m", ##__VA_ARGS__)
#define gprintf(fmt, ...) fprintf(stdout, "\033[1;32m" fmt "\033[0m", ##__VA_ARGS__)
#endif

#pragma pack(push,1)
typedef struct _fip_entry {
    char uuid[16];
    uint64_t file_offset;
    uint64_t file_size;
    uint64_t flags;
} fip_entry;

typedef struct _fip_header {
    uint64_t magic; // 0x12345678AA640001 in le
    uint64_t serial;
    fip_entry entries[0];
} fip_header;
#pragma pack(pop)

typedef struct _search_pattern {
    const char *pattern;
    const char *mask;
    size_t pattern_size;
} search_pattern;
#define EMPTY_PATTERN { NULL, NULL, 0 }
#define SEARCH_PATTERN(pattern, mask) { pattern, mask, sizeof(pattern) - 1 }

const search_pattern stub_patterns[] = {
    // these strings is in executable section of EL3 code
    // we use this as stub payload store
    SEARCH_PATTERN("Unhandled Interrupt Exception in EL3.\n", NULL),
    EMPTY_PATTERN
};

const search_pattern init_pattern[] = {
    {
        (const char[]){
            // 0x00, 0x00, 0x90, 0xD2, // mov     x0, 0x8000
            // 0x20, 0x00, 0xA0, 0xF2, // movk    x0, 1, lsl 16
            // 0x20, 0x13, 0x1E, 0xD5, // msr     MDCR_EL3, x0
            0xFF, 0x44, 0x03, 0xD5, // msr     DAIFClr, 4
            0x00, 0x00, 0x80, 0xD2, // mov     x0, 0 -> will be replaced call to stub
            0x40, 0x11, 0x1E, 0xD5 // msr     CPTR_EL3, x0
        },
        NULL,
        12
    },
    EMPTY_PATTERN,
};

const char stub_text[] = {
    0x00, 0x00, 0x90, 0xd2, // mov x0, 0x8000
    0x00, 0x3c, 0x50, 0xd3, // lsl x0, x0, #48
    // x0 now is 0x8000000000000000
    // set MPAM3_EL3: set to x0, which is MPAMEN = 1, enable MPAM
    0x00, 0xa5, 0x1e, 0xd5, // msr S3_6_C10_C5_0, x0
    0xDF, 0x3F, 0x03, 0xD5, // isb
    0x00, 0x00, 0x80, 0xD2, // mov     x0, 0
    0xc0, 0x03, 0x5f, 0xd6 // ret
};

char *search_pattern_in_buffer(const search_pattern *patterns, const char *buf, size_t buf_size) {
    for (int i = 0; patterns[i].pattern != NULL; i++) {
        const search_pattern *p = &patterns[i];
        for (size_t j = 0; j <= buf_size - p->pattern_size; j++) {
            bool match = true;
            for (size_t k = 0; k < p->pattern_size; k++) {
                if (p->mask) {
                    if ((buf[j + k] & p->mask[k]) != p->pattern[k]) {
                        match = false;
                        break;
                    }
                } else {
                    if (buf[j + k] != p->pattern[k]) {
                        match = false;
                        break;
                    }
                }
            }
            if (match) {
                return (char *)(buf + j);
            }
        }
    }
    return NULL;
}

int patch_el3(size_t size, char *buf) {
    int ret = 1;

    fip_header *header = (fip_header *)buf;
    if (header->magic != 0x12345678AA640001) {
        yprintf("Invalid FIP header magic: 0x%016lx, bad dump or mismatch endian (this tool is only for little endian)\n", header->magic);
        return 1;
    }

    // find BL31 image entry
    fip_entry *bl31_entry = NULL;
    for (int i = 0; i < 16; i++) {
        fip_entry *entry = &header->entries[i];
        if (memcmp(entry->uuid, "\x47\xD4\x08\x6D\x4C\xFE\x98\x46\x9B\x95\x29\x50\xCB\xBD\x5A\x00", 16) == 0) {
            bl31_entry = entry;
            break;
        }
    }
    if (!bl31_entry) {
        yprintf("BL31 entry not found in FIP\n");
        return 1;
    }

    printf("BL31 entry found at offset 0x%08lx, size 0x%08lx\n", bl31_entry->file_offset, bl31_entry->file_size);

    // find pattern for storing stub
    const search_pattern *stub_pattern = NULL;
    char *stub_start = NULL;
    for (int i = 0; stub_patterns[i].pattern != NULL; i++) {
        stub_start = search_pattern_in_buffer(&stub_patterns[i], buf + bl31_entry->file_offset, bl31_entry->file_size);
        if (stub_start) {
            stub_pattern = &stub_patterns[i];
            break;
        }
    } 
    if (!stub_pattern || !stub_start) {
        yprintf("Stub pattern not found in BL31 image\n");
        ret = 1;
        goto end;
    }
    printf("Found pattern '%s' at offset 0x%08lx\n", stub_pattern->pattern, stub_start - buf);

    // align stub_start to 4 bytes boundary ( relative to the start of BL31 image )
    stub_start = buf + bl31_entry->file_offset + (((stub_start - (buf + bl31_entry->file_offset)) + 3) & ~3);
    printf("Aligned stub start to offset 0x%08lx\n", stub_start - buf);


    int patched = 0;
    while (true) {
        // find init sequence in BL31 image
        char *init_start = search_pattern_in_buffer(init_pattern, buf + bl31_entry->file_offset, bl31_entry->file_size);
        if (!init_start) {
            if (!patched) {
                yprintf("Init pattern not found in BL31 image\n");
                ret = 1;
                break;
            } else {
                ret = 0;
                break;
            }
        }
        printf("Found init pattern at offset 0x%08lx\n", init_start - buf);

        // generate bl instruction to call stub
        // BL is at init_start + 4, offset is relative to BL's PC
        intptr_t bl_offset = (stub_start - (init_start + 4)) / 4;
        uint32_t bl_instr = 0x94000000 | (bl_offset & 0x3FFFFFF);

        // patch init sequence to call stub
        memcpy(init_start + 4, &bl_instr, sizeof(bl_instr));
    }

    // copy stub code to stub_start
    memcpy(stub_start, stub_text, sizeof(stub_text));

    ret = 0;
end:
    return ret;
}

#ifndef IN_UEFI
int main (int argc, char **argv) {
    int ret = 1;
    int fdr = -1, fdw = -1;
    char *buf = NULL;
    struct stat st;

    printf("This patch is for D920S10 BIOS, to patch EL3 code to enable MPAM\n");
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <bios dump as input_file> <patched dump as output_file>\n", argv[0]);
        goto end;
    }

    if (faccessat(AT_FDCWD, argv[1], F_OK, 0) != 0) {
        fprintf(stderr, "Input file %s does not exist\n", argv[1]);
        goto end;
    }
    if (faccessat(AT_FDCWD, argv[2], F_OK, 0) == 0) {
        fprintf(stderr, "Output file %s already exists\n", argv[2]);
        goto end;
    }

    for (int i = 0; i < 3; i++) {
        fprintf(stderr, "This patch is ABSOLUTELY NO WARRANTY\n");
        fprintf(stderr, "AT YOUR OWN RISK\n");
        sleep(1);
    }

    fdr = open(argv[1], O_RDONLY);
    if (fdr < 0) {
        perror("open input file");
        goto end;
    }

    // the BL31 is at 0xba0000
    if (lseek(fdr, 0xba0000, SEEK_SET) != 0xba0000) {
        perror("seek input file");
        goto end;
    }
    if (fstat(fdr, &st) != 0) {
        perror("fstat input file");
        goto end;
    }

    buf = malloc(0x100000);
    if (!buf) {
        perror("malloc");
        goto end;
    }

    ssize_t n = read(fdr, buf, 0x100000);
    if (n < 0) {
        perror("read input file");
        goto end;
    }

    if (patch_el3(0x100000, buf)) {
        fprintf(stderr, "Patch failed, file not changed\n");
        goto end;
    }

    fdw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdw < 0) {
        perror("open output file");
        goto end;
    }

    // copy file 0 - 0xba0000
    if (lseek(fdr, 0, SEEK_SET) != 0) {
        perror("seek input file");
        goto end;
    }
    if (sendfile(fdw, fdr, NULL, 0xba0000) != 0xba0000) {
        perror("copy input file to output file");
        goto end;
    }
    // write patched BL31
    if (write(fdw, buf, 0x100000) != 0x100000) {
        perror("write patched BL31 to output file");
        goto end;
    }
    // write remaining file 0xbc0000 - end
    if (lseek(fdr, 0xba0000 + 0x100000, SEEK_SET) != 0xba0000 + 0x100000) {
        perror("seek input file");
        goto end;
    }
    if (sendfile(fdw, fdr, NULL, st.st_size - 0xba0000 - 0x100000) != st.st_size - 0xba0000 - 0x100000) {
        perror("copy remaining input file to output file");
        goto end;
    }

    ret = 0;
end:
    if (buf) {
        free(buf);
    }
    if (fdr >= 0) {
        close(fdr);
    }
    if (fdw >= 0) {
        close(fdw);
    }

    return ret;
}
#endif
