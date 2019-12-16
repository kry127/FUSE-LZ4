#include <unistd.h>

struct lz4_block_descriptor {
    size_t block_size;
    size_t data_size;
};