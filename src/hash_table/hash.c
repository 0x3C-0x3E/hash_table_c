#include "hash.h"

uint32_t str_hash(const uint8_t *buffer, const size_t buffer_size) {
    uint32_t result = 5381;
    for (size_t i = 0; i < buffer_size; ++i) {
        result = result * 13 + (uint32_t)buffer[i];
    }

    return result;
}
