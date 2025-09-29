#include "hash_table.h"
#include <stdbool.h>

uint32_t hash(uint8_t *buffer, size_t buffer_size) {

    uint32_t result = 5381;
    for (size_t i = 0; i < buffer_size; ++i) {
        result = result * 13 + (uint32_t)buffer[i];
    }

    return result;
}

int hash_table_init(HashTable* ht) {
    *ht = (HashTable) {
        .capacity = INIT_TABLE_CAP,
        .count = 0,
    };
    
    ht->table = malloc(sizeof(KV) * ht->capacity);

    if (ht->table == NULL) {
        printf("[ERR] Malloc failed\n");
        return 1;
    }
        
    return 0;
}

bool hash_table_is_occupied(const HashTable *ht, size_t index) {
    if (index >= ht->count)
        return false;

    KV* kv = &ht->table[index];
    if (kv->occupied)
        return true;

    return false;
}

int hash_table_insert(HashTable *ht, size_t index, const KV *kv);

void hash_table_cleanup(HashTable* ht) {
    free(ht->table);
}
