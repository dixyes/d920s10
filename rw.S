
.text
.global read32

/* uint32_t read32(uintptr_t addr); */
read32:
    ldaxr w0, [x0]
    ret

/* void write32(uintptr_t addr, uint32_t val); */
write32:
    stlr w1, [x0]
    ret
