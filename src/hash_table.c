#include "hash_table.h"
#include <stdbool.h>
#include <string.h>

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

static bool buffer_compare(uint8_t *buff1, size_t buff1_size, uint8_t *buff2, size_t buff2_size) {
    if (buff1_size != buff2_size) {
        return false;
    }
    for (size_t i = 0; i < buff1_size; ++i) {
        if (buff1[i] != buff2[i]) {
            return false;
        }
    }
    return true;
}

int hash_table_insert(HashTable *ht, const KV *kv) {
    if ((ht->count + 1) > ht->capacity * 0.75) {
        if (resize_table(ht) == 1) {
            return 1;
        }
    }

    uint32_t index = str_hash(kv->key, kv->key_size) % ht->capacity;
    
    while (ht->table[index].occupied) {
        if (buffer_compare(kv->key, kv->key_size, ht->table[index].key, ht->table[index].key_size)) {
            break;
        }
        index = (index + 1) % ht->capacity;
    }
    
    memcpy(&ht->table[index], kv, sizeof(KV));

    ht->count ++;

    return 0;
}

int hash_table_update(HashTable *ht, const KV *kv);

int hash_table_append(HashTable *ht, const KV *kv);

int hash_table_get(HashTable *ht, const uint8_t *key, uint8_t key_size, KV *output_kv) {
    uint32_t index = str_hash(key, key_size) % ht->capacity;
    if (!ht->table[index].occupied) {
        return 1;
    }

    *output_kv = ht->table[index];
    
    return 0;
}

size_t hash_table_get_kv_index(HashTable *ht, KV *kv) {
    size_t index;
    for (index = 0; index < ht->capacity; ++index) {
        if (memcmp(&ht->table[index], kv, sizeof(KV)) == 0) {
            return index;
        }
    }
    return index;
}

int hash_table_set(HashTable *ht, const KV *kv);

void hash_table_dump(HashTable *ht) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        KV *kv = &ht->table[i];
        if (!kv->occupied) {
            continue;
        }

        printf("index %zu: {key: '", i);
        for (size_t j = 0; j < kv->key_size; ++j) {
            printf("%c", kv->key[j]);
        }
        printf("', value: '%d'}", kv->value);

        printf("\n");
    }
}

void hash_table_dump_kv(HashTable *ht, KV* kv) {
    printf("index %zu: {key: '", hash_table_get_kv_index(ht, kv));
    for (size_t j = 0; j < kv->key_size; ++j) {
        printf("%c", kv->key[j]);
    }
    printf("', value: '%d'}", kv->value);

    printf("\n");
}

void hash_table_cleanup(HashTable* ht) {
    free(ht->table);
}
