#include "hash_table.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

ht_return hash_table_init(HashTable* ht) {
    *ht = (HashTable) {
        .capacity = HT_INIT_CAP,
        .count = 0,
    };

    ht->table = malloc(sizeof(KV) * ht->capacity);
    
    if (ht->table == NULL) {
        printf("[ERR] Malloc failed\n");
        return HT_ERROR;
    }

    memset(ht->table, 0, sizeof(KV) * ht->capacity);
    return HT_SUCESS;
}

static bool key_compare(const Key* k1, const Key* k2) {
    if (k1->key_size != k2->key_size) {
        return false;
    }

    if (memcmp(k1->key, k2->key, k1->key_size) == 0) {
        return true;
    }

    return false;
}

static ht_return get_index_from_key(HashTable* ht, const Key* key, uint32_t* found_index) {
    uint32_t index = str_hash(key->key, key->key_size) % ht->capacity;
    uint32_t starting_index = index;

    if (!ht->table[index].occupied) {
        *found_index = index;
        return HT_NOT_IN_TABLE;
    }
    
    while (ht->table[index].occupied) {
        if (key_compare(key, &ht->table[index].key)) {
            *found_index = index;
            return HT_SUCESS;
        }

        index = (index + 1) % ht->capacity;
        if (index == starting_index) {
            *found_index = index;
            return HT_NOT_IN_TABLE;
        }
    }

    *found_index = index;
    return HT_SUCESS;
}

static ht_return resize_table(HashTable *ht) {
    size_t new_capacity = ht->capacity * 2;
    if (new_capacity == 0) new_capacity = sizeof(KV);

    KV* new_table = calloc(new_capacity, sizeof(KV));
    if (new_table == NULL) {
        printf("[ERR] CALLOC FAILED\n");
        return HT_ERROR;
    }

    for (size_t i = 0; i < ht->capacity; ++i) {
        KV* kv = &ht->table[i];
        if (kv->occupied) {
            uint32_t index = str_hash(kv->key.key, kv->key.key_size) % new_capacity;
            uint32_t starting_index = index;

            while (new_table[index].occupied) {
                index = index + 1;
                if (index == starting_index) {
                    return HT_TABLE_OVERFLOW;
                }
            }

            memcpy(&new_table[index], kv, sizeof(KV));
        }
    }

    free(ht->table);
    ht->table = new_table;
    ht->capacity = new_capacity;

    return HT_SUCESS;
}

ht_return check_resize(HashTable* ht) {
    if ((ht->count + 1) > ht->capacity * 0.75) {
        int ret;
        if ((ret = resize_table(ht)) != HT_SUCESS) {
            return ret;
        }
    }
    return HT_SUCESS;
}

ht_return hash_table_insert(HashTable* ht, const Key* key, const Value* value) {
    int ret;
    if ((ret = check_resize(ht)) != HT_SUCESS) {
        return ret;
    }

    KV new_kv = (KV) {
        .key = *key,
        .value = *value,
        .occupied = true,
    };
    
    uint32_t index;
    ret = get_index_from_key(ht, key, &index);
    if (ret == HT_NOT_IN_TABLE) {
        ht->count ++;
    }
       
    memcpy(&ht->table[index], &new_kv, sizeof(KV));

    return HT_SUCESS;
}

ht_return hash_table_update(HashTable* ht, const Key* key, const Value* value) {
    KV new_kv = (KV) {
        .key = *key,
        .value = *value,
        .occupied = true,
    };
    
    uint32_t index;
    int ret = get_index_from_key(ht, key, &index);
    if (ret == HT_SUCESS) {
        memcpy(&ht->table[index], &new_kv, sizeof(KV));
        return HT_SUCESS;
    }

    return HT_NOT_IN_TABLE;
}

ht_return hash_table_append(HashTable* ht, const Key* key, const Value* value) {
    int ret;
    if ((ret = check_resize(ht)) != HT_SUCESS) {
        return ret;
    }

    KV new_kv = (KV) {
        .key = *key,
        .value = *value,
        .occupied = true,
    };
    
    uint32_t index;
    ret = get_index_from_key(ht, key, &index);
    if (ret == HT_SUCESS) {
        return HT_IN_TABLE;
    }
       
    memcpy(&ht->table[index], &new_kv, sizeof(KV));
    ht->count ++;

    return HT_SUCESS;
}

ht_return hash_table_get(HashTable* ht, const Key* key, Value* value) {
    uint32_t index;
    int ret = get_index_from_key(ht, key, &index);
    if (ret == HT_SUCESS) {
        // memcpy(kv, &ht->table[index], sizeof(KV));
        *value = ht->table[index].value;
        return HT_SUCESS;
    }

    return HT_NOT_IN_TABLE;
}

void hash_table_dump_kv(HashTable *ht, KV* kv) {
    uint32_t index;

    if (get_index_from_key(ht, &kv->key, &index) == HT_NOT_IN_TABLE) {
        return;
    }

    printf("index %u: {key: '", index);
    for (size_t j = 0; j < kv->key.key_size; ++j) {
        printf("%c", kv->key.key[j]);
    }
    printf("', value: '%d'}", kv->value);
    printf("\n");
}

void hash_table_dump(HashTable *ht) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        KV *kv = &ht->table[i];
        if (!kv->occupied) {
            continue;
        }

        hash_table_dump_kv(ht, kv);
    }
}

void hash_table_cleanup(HashTable* ht) {
    free(ht->table);
}
