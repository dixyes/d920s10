

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "aml.c"

static inline aml_node *aml_op_node(uint16_t opcode, uint16_t num_children) {
    return aml_alloc_node(opcode, num_children);
}

int main () {
    aml_node *root, *if_node, *ext1, *ext2, *ext3, *scope1, *scope2, *scope3;

    // --- build from leaves up ---

    // ExternalOp 1 (_SB.I2C0.COM1, DeviceObj)
    ext1 = aml_op_node(AML_OPCODE_ExternalOp, 3);
    ext1->children[0] = aml_data_node("\\/\x03_SB_I2C0COM1", 15);
    ext1->children[1] = aml_data_node("\x06", 1);
    ext1->children[2] = aml_data_node("\x00", 1);

    // ExternalOp 2 (_SB.SPI0.COM2, DeviceObj)
    ext2 = aml_op_node(AML_OPCODE_ExternalOp, 3);
    ext2->children[0] = aml_data_node("\\/\x03_SB_SPI0COM2", 15);
    ext2->children[1] = aml_data_node("\x06", 1);
    ext2->children[2] = aml_data_node("\x00", 1);

    // ExternalOp 3 (_SB.WIFI, DeviceObj)
    ext3 = aml_op_node(AML_OPCODE_ExternalOp, 3);
    ext3->children[0] = aml_data_node("\\._SB_WIFI", 10);
    ext3->children[1] = aml_data_node("\x06", 1);
    ext3->children[2] = aml_data_node("\x00", 1);

    // If (0) { External x3 }
    if_node = aml_op_node(AML_OPCODE_IfOp, 4);
    if_node->children[0] = aml_op_node(AML_OPCODE_ZeroOp, 0);
    if_node->children[1] = ext1;
    if_node->children[2] = ext2;
    if_node->children[3] = ext3;

    // Scope (\_SB.I2C0.COM1) { Name(_HID,"DIXY0001") }
    scope1 = aml_op_node(AML_OPCODE_ScopeOp, 2);
    scope1->children[0] = aml_data_node("\\/\x03_SB_I2C0COM1", 15);
    scope1->children[1] = aml_data_node("\x08_HID\x0d" "DIXY0001\x00", 15);

    // Scope (\_SB.SPI0.COM2) { Name(_HID,"DIXY0002") }
    scope2 = aml_op_node(AML_OPCODE_ScopeOp, 2);
    scope2->children[0] = aml_data_node("\\/\x03_SB_SPI0COM2", 15);
    scope2->children[1] = aml_data_node("\x08_HID\x0d" "DIXY0002\x00", 15);

    // Scope (\_SB.WIFI) { Name(_STA,0) }
    scope3 = aml_op_node(AML_OPCODE_ScopeOp, 2);
    scope3->children[0] = aml_data_node("\\._SB_WIFI", 10);
    scope3->children[1] = aml_data_node("\x08_STA\x00", 6);

    // Root
    root = aml_op_node(AML_OPCODE__ROOT, 4);
    root->children[0] = if_node;
    root->children[1] = scope1;
    root->children[2] = scope2;
    root->children[3] = scope3;

    size_t size = aml_node_size(root);
    printf("aml_node_size: %zu\n", size);
    char *buffer = malloc(size);
    if (!buffer) {
        fprintf(stderr, "failed to malloc %zu bytes: %d\n", size, errno);
        aml_free_node(root, true);
        return 1;
    }

    int fd = open("w510_repro.aml", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "failed to open w510_repro.aml: %d\n", errno);
        aml_free_node(root, true);
        free(buffer);
        return 1;
    }

    int32_t ret = aml_node_serialize(buffer, 0, root);
    if (ret < 0) {
        fprintf(stderr, "aml_node_serialize failed: %d\n", ret);
        aml_free_node(root, true);
        free(buffer);
        close(fd);
        return 1;
    }

    // write header
    char header[] = {
        0x53, 0x53, 0x44, 0x54, 0xaa, 0x00, 0x00, 0x00, 0x02, 0xeb, 0x44, 0x49,
        0x58, 0x59, 0x45, 0x53, 0x44, 0x49, 0x58, 0x59, 0x45, 0x53, 0x30, 0x32,
        0x00, 0x00, 0x00, 0x00, 0x49, 0x4e, 0x54, 0x4c, 0x28, 0x06, 0x23, 0x20
    };
    if (write(fd, header, sizeof(header)) != (ssize_t)sizeof(header)) {
        fprintf(stderr, "failed to write header: %d\n", errno);
    }

    if (write(fd, buffer, size) != (ssize_t)size) {
        fprintf(stderr, "failed to write body: %d\n", errno);
    }

    close(fd);
    free(buffer);
    aml_free_node(root, true);
    return 0;
}