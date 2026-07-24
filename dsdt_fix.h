#ifndef _DSDT_FIX_H_
#define _DSDT_FIX_H_

#include "definations.h"

typedef struct _devpath_list {
    struct _devpath_list *next;
    size_t name_size;
    char *name;
} devpath_list;

typedef struct _dsdt_walker_data {
    devpath_list *offline_ddrcs; // out: offline DDRCs
    devpath_list *missing_hids; // out: missing HID devices
    uintptr_t online_ddrcs_base[MAX_SOCKET * MAX_CHANNEL]; // in: online DDRCs base addresses
} dsdt_walker_data;

static void devpath_list_append_uacpi_object_names(
    devpath_list **list, const uacpi_object_name *names, uint8_t depth
) {
    size_t size;
    char *name;

    // generate names and size
    if (depth == 2) {
        // use RootChar '\\', DualNamePrefix '.', parts to encode it
        size = 10;
        name = (char *)malloc(size);
        if (!name)
            return;
        name[0] = '\\';
        name[1] = '.';
        memcpy(name + 2, names, 8);
    } else {
        // use RootChar '\\', MultiNamePrefix '/', num of parts, parts to encode it
        size = 3 + (depth) * 4;
        name = (char *)malloc(size);
        if (!name)
            return;
        name[0] = '\\';
        name[1] = '/';
        name[2] = (char)depth;
        memcpy(name + 3, names, 4 * depth);
    }

    // append to list
    devpath_list **p_new_node = list;
    // find the end of the list
    while (*p_new_node) {
        p_new_node = &(*p_new_node)->next;
    }
    *p_new_node = (devpath_list *)malloc(sizeof(devpath_list));
    if (!*p_new_node) {
        return;
    }
    (*p_new_node)->next = NULL;
    (*p_new_node)->name_size = size;
    (*p_new_node)->name = name;
}


#endif /* _DSDT_FIX_H_ */
