#include "hash_table.h"
#include <stdbool.h>
#include <string.h>

uint32_t str_hash(uint8_t *buffer, size_t buffer_size) {
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

    memset(ht->table, 0, sizeof(KV) * ht->capacity);
    return 0;
}

int hash_table_is_occupied(const HashTable *ht, size_t index) {
    if (index >= ht->capacity)
        return -1;

    KV* kv = &ht->table[index];
    if (kv->occupied)
        return 1;

    return 0;
}

static int resize_table(HashTable *ht) {
    size_t new_capacity = ht->capacity * 2;
    if (new_capacity == 0) new_capacity = sizeof(KV);

    KV *new_table = calloc(new_capacity, sizeof(KV));
    if (new_table == NULL) {
        printf("[ERR] CALLOC FAILED\n");
        return 1; 
    }

    for (size_t i = 0; i < ht->capacity; ++i) {
        KV *kv = &ht->table[i];
        if (kv->occupied) {
            uint32_t index = str_hash(kv->key, kv->key_size) % new_capacity;

            while (new_table[index].occupied) {
                index = (index + 1) % new_capacity;
            }

            memcpy(&new_table[index], kv, sizeof(KV));
        }
    }

    free(ht->table);
    ht->table = new_table;
    ht->capacity = new_capacity;

    return 0;
}

int hash_table_insert(HashTable *ht, const KV *kv) {
    if ((ht->count + 1) > ht->capacity * 0.75) {
        if (resize_table(ht) == 1) {
            return 1;
        }
    }

    uint32_t index = str_hash(kv->key, kv->key_size) % ht->capacity;
    
    while (ht->table[index].occupied) {
        index = (index + 1) % ht->capacity;
    }
    
    memcpy(&ht->table[index], kv, sizeof(KV));

    ht->count ++;

    return 0;
}

void hash_table_dump(HashTable *ht) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        KV *kv = &ht->table[i];
        if (!kv->occupied) {
            continue;
        }

        printf("INDEX %zu: ", i);
        for (size_t j = 0; j < kv->key_size; ++j) {
            printf("%c", kv->key[j]);
        }
        printf("| VALUE: %d", kv->value);

        printf("\n");
    }
}

void hash_table_cleanup(HashTable* ht) {
    free(ht->table);
}
