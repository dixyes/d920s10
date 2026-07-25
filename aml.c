
// minimal AML serialization implementation for dynamic SSDT generation

#include "definitions.h"
#include "aml.h"

size_t aml_node_size(const aml_node *node) {
    size_t size = 0;
    if (!node) {
        // NULL node, the node is removed
        return 0;
    }

    // special: AML_OPCODE__DATA for representing raw bytes
    // just use data_size and data
    if (node->opcode == AML_OPCODE__DATA) {
        return node->data_size; // for data
    }
    // special: AML_OPCODE__ROOT for representing root node, simple node list
    if (node->opcode == AML_OPCODE__ROOT) {
        for (uint16_t i = 0; i < node->children_num; i++) {
            const aml_node *child = node->children[i];
            size_t ret = aml_node_size(child);
            if (ret == (size_t)AML_ERROR_NOT_IMPLEMENTED) {
                return ret;
            }
            size += ret;
        }
        return size;
    }

    // children sizes
    for (uint16_t i = 0; i < node->children_num; i++) {
        const aml_node *child = node->children[i];
        size_t ret = aml_node_size(child);
        if (ret == (size_t)AML_ERROR_NOT_IMPLEMENTED) {
            return ret;
        }
        size += ret;
    }

    // specific node sizes
    switch (node->opcode) {
        case AML_OPCODE_ScopeOp:
            // 0x10, ?b pkglength, ?b terms
            /* fallthrough */
        case AML_OPCODE_IfOp:
            // 0xa0, ?b pkglength, ?b cond, ?b terms
            /* fallthrough */

            // container nodes
            // pkglength
            if (size < 63) {
                // 1-byte length encoding, size of children
                size += 1;
            } else if (size < 4094) {
                // 2-byte length encoding, size of children + 1
                size += 2;
            } else if (size < 1048573) {
                // 3-byte length encoding, size of children + 2
                size += 3;
            } else {
                // 4-byte length encoding, size of children + 3
                size += 4;
            }
        case AML_OPCODE_ExternalOp:
            // 0x15, ?b NameString(children[0]), 1b ObjectType(as AML_OPCODE__DATA children[1]), ?b ArgumentCount(as children[2])
            /* fallthrough */
        case AML_OPCODE_ZeroOp:
            // 0x00
            // no pkglength
            break;
        default:
            // not implemented
            return (size_t)AML_ERROR_NOT_IMPLEMENTED;
    }

    // size for opcode
    size += 1; // for opcode
    if (node->opcode > 0xff) {
        size += 1; // for 2byte opcode
    }

    return size;
}

/*
  returns 0 for success
  negtive for error
  positive for bytes written
*/
int32_t aml_node_serialize(
    char *buffer,
    size_t pos, // pass 0 for top call
    const aml_node *node
) {
    if (!node) {
        // NULL node, nothing to serialize
        return 0;
    }

    // special: AML_OPCODE__DATA for representing raw bytes
    if (node->opcode == AML_OPCODE__DATA) {
        memcpy(buffer + pos, node->data, node->data_size);
        return 0;
    }
    if (node->opcode == AML_OPCODE__ROOT) {
        for (uint16_t i = 0; i < node->children_num; i++) {
            const aml_node *child = node->children[i];
            int32_t ret = aml_node_serialize(buffer, pos, child);
            if (ret < 0) {
                return ret;
            }
            pos += aml_node_size(child);
        }
        return 0;
    }

    // little-endian opcode
    size_t children_size = aml_node_size(node) - 1;
    buffer[pos++] = (char)(node->opcode & 0xff);
    if (node->opcode > 0xff) {
        children_size -= 1; // for 2byte opcode
        buffer[pos++] = (char)((node->opcode >> 8) & 0xff);
    }

    switch (node->opcode) {
        case AML_OPCODE_ScopeOp:
            /* fallthrough */
        case AML_OPCODE_IfOp:
            // container nodes
            // pkglength
            if (children_size < 64) {
                // 1-byte length encoding
                buffer[pos++] = (char)(children_size & 0xff);
            } else if (children_size < 4096) {
                // 2-byte length encoding
                buffer[pos++] = (char)(0x40 | (children_size & 0x0f));
                buffer[pos++] = (char)((children_size >> 4) & 0xff);
            } else if (children_size < 1048576) {
                // 3-byte length encoding
                buffer[pos++] = (char)(0x80 | (children_size & 0x0f));
                buffer[pos++] = (char)((children_size >> 4) & 0xff);
                buffer[pos++] = (char)((children_size >> 12) & 0xff);
            } else {
                // 4-byte length encoding
                buffer[pos++] = (char)(0xc0 | (children_size & 0x0f));
                buffer[pos++] = (char)((children_size >> 4) & 0xff);
                buffer[pos++] = (char)((children_size >> 12) & 0xff);
                buffer[pos++] = (char)((children_size >> 20) & 0xff);
            }
        case AML_OPCODE_ExternalOp:
            /* fallthrough */
        case AML_OPCODE_ZeroOp:
            break;
        default:
            // not implemented
            return AML_ERROR_NOT_IMPLEMENTED;
    }

    // write children
    for (uint16_t i = 0; i < node->children_num; i++) {
        const aml_node *child = node->children[i];
        int32_t ret = aml_node_serialize(buffer, pos, child);
        if (ret < 0) {
            return ret;
        }
        pos += aml_node_size(child);
    }

    return 0;
}
