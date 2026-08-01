
#pragma push_macro("_MSC_VER")
#undef _MSC_VER
#define UACPI_OVERRIDE_TYPES
#include "uacpi/uacpi.h"
#pragma pop_macro("_MSC_VER")
#include "tlsf.h"

extern const efi_guid_t acpi_guid;
extern const efi_guid_t acpi2_guid;

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address) {
    efi_configuration_table_t *p_table;
    for (uintn_t i = 0; i < ST->NumberOfTableEntries; i++) {
        p_table = &ST->ConfigurationTable[i];
        if (
            memcmp(&p_table->VendorGuid, &acpi_guid, sizeof(acpi_guid)) &&
            memcmp(&p_table->VendorGuid, &acpi2_guid, sizeof(acpi2_guid))
        ) {
            continue;
        }

        if (memcmp("RSD PTR ", (void *)(p_table->VendorTable), 8)) {
            continue;
        }

        *out_rsdp_address = (uacpi_phys_addr)(uintptr_t)p_table->VendorTable;
        return UACPI_STATUS_OK;
    }

    return UACPI_STATUS_NOT_FOUND;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
    (void) len;
    return (void*)((uintptr_t)addr);
}

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
    (void)addr;
    (void)len;
}

void uacpi_kernel_log(uacpi_log_level lv, const char *fmt) {
    // va_list args;
    // va_start(args, fmt);
    if (lv >= UACPI_LOG_INFO) {
        // no logging for info and above, to avoid spamming the console
        return;
    }
    switch (lv) {
    case UACPI_LOG_ERROR:
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_BRIGHT);
        printf("uACPI ERROR: %s", fmt);
        break;
    case UACPI_LOG_WARN:
        ST->ConOut->SetAttribute(ST->ConOut, EFI_YELLOW | EFI_BRIGHT);
        printf("uACPI WARNING: %s", fmt);
        break;
    case UACPI_LOG_INFO:
        ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_GREEN | EFI_BLUE);
        printf("uACPI INFO: %s", fmt);
        break;
    case UACPI_LOG_TRACE:
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BLUE | EFI_BRIGHT);
        printf("uACPI TRACE: %s", fmt);
        break;
    case UACPI_LOG_DEBUG:
        ST->ConOut->SetAttribute(ST->ConOut, EFI_GREEN | EFI_BLUE);
        printf("uACPI DEBUG: %s", fmt);
        break;
    }
    // vprintf(fmt, args);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_GREEN | EFI_BLUE);
    // va_end(args);
}

uacpi_handle uacpi_kernel_create_mutex(void) {
    return (uacpi_handle)1;
}

void uacpi_kernel_free_mutex(uacpi_handle handle) {
    (void)handle;
    // do nothing
}

uacpi_status uacpi_kernel_acquire_mutex(uacpi_handle handle, uacpi_u16 timeout) {
    (void)handle;
    (void)timeout;
    // do nothing
    return UACPI_STATUS_OK;
}

void uacpi_kernel_release_mutex(uacpi_handle handle) {
    (void)handle;
    // do nothing
}

static void *arena = NULL;
static tlsf_t uacpi_kernel_tlsf;

#define UACPI_KERNEL_ARENA_SIZE (64 * 1024 * 1024) // 64 MB

static int init_arena_once(void) {
    if (uacpi_kernel_tlsf)
        return 0;

    efi_status_t ret = BS->AllocatePool(
        EfiBootServicesData, UACPI_KERNEL_ARENA_SIZE, (void**)&arena);
    if (ret != EFI_SUCCESS) {
        printf("uACPI: AllocatePool(%lu MB) FAILED ret=%lu\n",
               (unsigned long)(UACPI_KERNEL_ARENA_SIZE / 1024 / 1024), (unsigned long)ret);
        return -1;
    }

    uacpi_kernel_tlsf = tlsf_create_with_pool(arena, UACPI_KERNEL_ARENA_SIZE);
    if (!uacpi_kernel_tlsf) {
        printf("uACPI: tlsf_create_with_pool FAILED\n");
        return -1;
    }

    return 0;
}

uacpi_status uacpi_kernel_initialize(uacpi_init_level current_init_lvl) {
    (void)current_init_lvl;
    return init_arena_once() == 0 ? UACPI_STATUS_OK : UACPI_STATUS_OUT_OF_MEMORY;
}

void uacpi_kernel_deinitialize(void) {
    tlsf_destroy(uacpi_kernel_tlsf);
    uacpi_kernel_tlsf = NULL;

    efi_status_t ret = BS->FreePool(arena);
    if (ret != EFI_SUCCESS) {
        printf("uACPI kernel deinitialize failed to free arena: %d\n", ret);
    }
    arena = NULL;
}

void *uacpi_kernel_alloc(uacpi_size size) {
    if (init_arena_once() != 0) {
        printf("uacpi_kernel_alloc: arena init failed\n");
        return NULL;
    }
    return tlsf_malloc(uacpi_kernel_tlsf, size);
}

void uacpi_kernel_free(void *mem) {
    tlsf_free(uacpi_kernel_tlsf, mem);
}

uacpi_u64 uacpi_kernel_get_nanoseconds_since_boot(void) {
    static uacpi_u64 freq = 0;
    uacpi_u64 ticks;

    if (freq == 0) {
        __asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(freq));
    }
    __asm__ volatile("mrs %0, CNTPCT_EL0" : "=r"(ticks));

    return (ticks * 1000000000ULL) / freq;
}

void uacpi_kernel_sleep(uacpi_u64 msec) {
    // we are not really executing, just skip it
    (void)msec;
    BS->Stall(msec * 1000);
}

void uacpi_kernel_stall(uacpi_u8 usec) {
    // we are not really executing, just skip it
    (void)usec;
    BS->Stall(usec);
}

uacpi_handle uacpi_kernel_create_event(void) {
    return NULL;
}

void uacpi_kernel_free_event(uacpi_handle handle) {
    (void)handle;
    // do nothing
}

uacpi_bool uacpi_kernel_wait_for_event(uacpi_handle handle, uacpi_u16 timeout) {
    (void)handle;
    (void)timeout;
    // we are not really executing, just skip it
    return UACPI_TRUE;
}

void uacpi_kernel_signal_event(uacpi_handle handle) {
    (void)handle;
    // do nothing
}

void uacpi_kernel_reset_event(uacpi_handle handle) {
    (void)handle;
    // do nothing
}

uacpi_status uacpi_kernel_handle_firmware_request(uacpi_firmware_request *req) {
    (void)req;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_map(
    uacpi_io_addr base, uacpi_size len, uacpi_handle *out_handle
) {
    (void)len;

    *out_handle = (uacpi_handle)((uintptr_t)base);

    return UACPI_STATUS_OK;
}

void uacpi_kernel_io_unmap(uacpi_handle handle)
{
    (void)handle;
}

uacpi_status uacpi_kernel_pci_device_open(
    uacpi_pci_address address, uacpi_handle *out_handle
)
{
    return UACPI_STATUS_OK;
}

void uacpi_kernel_pci_device_close(uacpi_handle handle) {
    (void)handle;
}

uacpi_status uacpi_kernel_io_read8(
    uacpi_handle handle, uacpi_size offset, uacpi_u8 *out_value
) {
    *out_value = *(volatile uacpi_u8 *)((uacpi_u8 *)handle + offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_read16(
    uacpi_handle handle, uacpi_size offset, uacpi_u16 *out_value
) {
    *out_value = *(volatile uacpi_u16 *)((uacpi_u8 *)handle + offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_read32(
    uacpi_handle handle, uacpi_size offset, uacpi_u32 *out_value
) {
    *out_value = *(volatile uacpi_u32 *)((uacpi_u8 *)handle + offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_write8(
    uacpi_handle handle, uacpi_size offset, uacpi_u8 in_value
) {
    *(volatile uacpi_u8 *)((uacpi_u8 *)handle + offset) = in_value;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_write16(
    uacpi_handle handle, uacpi_size offset, uacpi_u16 in_value
) {
    *(volatile uacpi_u16 *)((uacpi_u8 *)handle + offset) = in_value;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_io_write32(
    uacpi_handle handle, uacpi_size offset, uacpi_u32 in_value
) {
    *(volatile uacpi_u32 *)((uacpi_u8 *)handle + offset) = in_value;
    return UACPI_STATUS_OK;
}

/* PCI config space — stub, never actually called in reduced hardware mode */
uacpi_status uacpi_kernel_pci_read8(
    uacpi_handle device, uacpi_size offset, uacpi_u8 *value
) {
    (void)device; (void)offset;
    *value = 0xFF;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read16(
    uacpi_handle device, uacpi_size offset, uacpi_u16 *value
) {
    (void)device; (void)offset;
    *value = 0xFFFF;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read32(
    uacpi_handle device, uacpi_size offset, uacpi_u32 *value
) {
    (void)device; (void)offset;
    *value = 0xFFFFFFFF;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write8(
    uacpi_handle device, uacpi_size offset, uacpi_u8 value
) {
    (void)device; (void)offset; (void)value;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write16(
    uacpi_handle device, uacpi_size offset, uacpi_u16 value
) {
    (void)device; (void)offset; (void)value;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_write32(
    uacpi_handle device, uacpi_size offset, uacpi_u32 value
) {
    (void)device; (void)offset; (void)value;
    return UACPI_STATUS_OK;
}

/* Spinlock — used by registers.c, needed for namespace_load to proceed */
uacpi_handle uacpi_kernel_create_spinlock(void) {
    return (uacpi_handle)1; /* non-NULL so uACPI doesn't think OOM */
}

void uacpi_kernel_free_spinlock(uacpi_handle handle) {
    (void)handle;
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle handle) {
    (void)handle;
    return 0;
}

void uacpi_kernel_unlock_spinlock(uacpi_handle handle, uacpi_cpu_flags flags) {
    (void)handle; (void)flags;
}

/* Thread ID — used by mutex.c for recursive lock owner tracking */
uacpi_thread_id uacpi_kernel_get_thread_id(void) {
    return (uacpi_thread_id)1; /* non-NONE, single-threaded anyway */
}

/* Interrupts — stub, never called in reduced hardware mode */
uacpi_interrupt_state uacpi_kernel_disable_interrupts(void) {
    return 0;
}

void uacpi_kernel_restore_interrupts(uacpi_interrupt_state state) {
    (void)state;
}

/* Interrupt handlers — stub, REDUCED_HARDWARE makes these unreachable */
uacpi_status uacpi_kernel_install_interrupt_handler(
    uacpi_u32 irq, uacpi_interrupt_handler handler, uacpi_handle ctx,
    uacpi_handle *out_irq_handle
) {
    (void)irq; (void)handler; (void)ctx;
    *out_irq_handle = UACPI_NULL;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_uninstall_interrupt_handler(
    uacpi_interrupt_handler handler, uacpi_handle irq_handle
) {
    (void)handler; (void)irq_handle;
    return UACPI_STATUS_OK;
}

/* Work scheduling — stub, never called without notify/events */
uacpi_status uacpi_kernel_schedule_work(
    uacpi_work_type type, uacpi_work_handler handler, uacpi_handle ctx
) {
    (void)type; (void)handler; (void)ctx;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_wait_for_work_completion(void) {
    return UACPI_STATUS_OK;
}