#ifndef _AML_H_
# define _AML_H_

typedef struct _aml_node {
    uint16_t opcode;
    union {
        struct {
            uint16_t children_num;
            struct _aml_node **children;
        };
        struct {
            uint16_t data_size;
            char *data;
        };
    };
} aml_node;

#define AML_OPCODE__DATA 0xff00
#define AML_OPCODE__ROOT 0xff01
#define AML_OPCODE_ZeroOp 0x00
#define AML_OPCODE_ScopeOp 0x10
#define AML_OPCODE_ExternalOp 0x15
#define AML_OPCODE_IfOp 0xa0

#ifdef ENOSYS
# define AML_ERROR_NOT_IMPLEMENTED -ENOSYS
#else
# define AML_ERROR_NOT_IMPLEMENTED -38
#endif
#ifdef ENOBUFS
# define AML_ERROR_INSUFFICIENT_BUFFER -ENOBUFS
#else
# define AML_ERROR_INSUFFICIENT_BUFFER -105
#endif
#ifdef EINVAL
# define AML_ERROR_INVALID_PARAMETER -EINVAL
#else
# define AML_ERROR_INVALID_PARAMETER -22
#endif

size_t aml_node_size(const aml_node *node);
int32_t aml_node_serialize(
    char *buffer,
    size_t pos, // pass 0 for top call
    const aml_node *node
);

static inline aml_node *aml_alloc_node(
    uint16_t opcode,
    uint16_t children_num_or_data_size
) {
    size_t size = sizeof(aml_node);
    switch (opcode) {
        case AML_OPCODE__DATA:
            size += children_num_or_data_size;
            break;
        default:
            size += sizeof(aml_node *) * children_num_or_data_size;
            break;
    }

    aml_node *node = malloc(size);
    if (!node) {
        return NULL;
    }
    node->opcode = opcode;
    
    switch (opcode) {
        case AML_OPCODE__DATA:
            node->data_size = children_num_or_data_size;
            node->data = (char*)(node + 1);
            break;
        default:
            node->children_num = children_num_or_data_size;
            node->children = (aml_node**)(node + 1);
            break;
    }

    memset(node + 1, 0, size - sizeof(aml_node));
    return node;
}

static inline void aml_free_node(aml_node *node, uint8_t recursive) {
    if (!node) {
        return;
    }
    if (recursive && node->opcode != AML_OPCODE__DATA) {
        for (uint16_t i = 0; i < node->children_num; i++) {
            aml_free_node(node->children[i], 1);
        }
    }
    free(node);
}

static inline aml_node *aml_data_node(const char *data, uint16_t size) {
    aml_node *n = aml_alloc_node(AML_OPCODE__DATA, size);
    if (n)
        memcpy(n->data, data, size);
    return n;
}

#endif /* _AML_H_ */
