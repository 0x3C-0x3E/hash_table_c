#include "hash_table.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


// int hash_table_insert(HashTable *ht, const KV *kv) {
//     if ((ht->count + 1) > ht->capacity * 0.75) {
//         if (resize_table(ht) == 1) {
//             return 1;
//         }
//     }
//
//     uint32_t index = str_hash(kv->key, kv->key_size) % ht->capacity;
//
//     while (ht->table[index].occupied) {
//         if (buffer_compare(kv->key, kv->key_size, ht->table[index].key, ht->table[index].key_size)) {
//             break;
//         }
//         index = (index + 1) % ht->capacity;
//     }
//
//     memcpy(&ht->table[index], kv, sizeof(KV));
//
//     ht->count ++;
//
//     return 0;
// }
//
// int hash_table_update(HashTable *ht, const KV *kv);
//
// int hash_table_append(HashTable *ht, const KV *kv);
//
// int hash_table_get(HashTable *ht, const uint8_t *key, uint8_t key_size, KV *output_kv) {
//     uint32_t index = str_hash(key, key_size) % ht->capacity;
//     if (!ht->table[index].occupied) {
//         return 1;
//     }
//
//     *output_kv = ht->table[index];
//
//     return 0;
// }
//
// size_t hash_table_get_kv_index(HashTable *ht, KV *kv) {
//     size_t index;
//     for (index = 0; index < ht->capacity; ++index) {
//         if (memcmp(&ht->table[index], kv, sizeof(KV)) == 0) {
//             return index;
//         }
//     }
//     return index;
// }
//
// int hash_table_set(HashTable *ht, const KV *kv);
//
// void hash_table_dump(HashTable *ht) {
//     for (size_t i = 0; i < ht->capacity; ++i) {
//         KV *kv = &ht->table[i];
//         if (!kv->occupied) {
//             continue;
//         }
//
//         printf("index %zu: {key: '", i);
//         for (size_t j = 0; j < kv->key_size; ++j) {
//             printf("%c", kv->key[j]);
//         }
//         printf("', value: '%d'}", kv->value);
//
//         printf("\n");
//     }
// }
//
// void hash_table_dump_kv(HashTable *ht, KV* kv) {
//     printf("index %zu: {key: '", hash_table_get_kv_index(ht, kv));
//     for (size_t j = 0; j < kv->key_size; ++j) {
//         printf("%c", kv->key[j]);
//     }
//     printf("', value: '%d'}", kv->value);
//
//     printf("\n");
// }
//

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

static ht_return get_index_from_key(HashTable* ht, const KV* kv, size_t* found_index) {
    uint32_t index = str_hash(kv->key.key, kv->key.key_size) % ht->capacity;

    if (ht->table[index].occupied && !key_compare(&kv->key, &ht->table[index].key)) {

        while (ht->table[index].occupied) {
            if (key_compare(&kv->key, &ht->table[index].key)) {
                break;
            }
            index = (index + 1) % ht->capacity;
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





ht_return hash_table_insert(HashTable* ht, const KV* kv) {
    if ((ht->count + 1) > ht->capacity * 0.75) {
        int ret;
        if ((ret = resize_table(ht)) != HT_SUCESS) {
            return ret;
        }
    }

    uint32_t index = str_hash(kv->key.key, kv->key.key_size) % ht->capacity;

    if (ht->table[index].occupied && !key_compare(&kv->key, &ht->table[index].key)) {
        ht->table[index].colliding = true;

        while (ht->table[index].occupied) {
            if (key_compare(&kv->key, &ht->table[index].key)) {
                break;
            }
            index = (index + 1) % ht->capacity;
        }
    }
    
    memcpy(&ht->table[index], kv, sizeof(KV));

    ht->count ++;

    return HT_SUCESS;
}

ht_return hash_table_update(HashTable* ht, const KV* kv) {
    uint32_t index = str_hash(kv->key.key, kv->key.key_size) % ht->capacity;

    if (ht->table[index].occupied && !key_compare(&kv->key, &ht->table[index].key)) {
        while (ht->table[index].occupied) {
            if (key_compare(&kv->key, &ht->table[index].key)) {
                break;
            }
            index = (index + 1) % ht->capacity;
        }
    }
    memcpy(&ht->table[index], kv, sizeof(KV));
    return HT_SUCESS;

    
    while (ht->table[index].occupied) {
        if (key_compare(&kv->key, &ht->table[index].key)) {
            break;
        }
        index = (index + 1) % ht->capacity;
    }
    
    memcpy(&ht->table[index], kv, sizeof(KV));

    ht->count ++;

    return HT_SUCESS;
}

ht_return hash_table_append(HashTable* ht, const KV* kv) {

}

ht_return hash_table_get(HashTable* ht, const Key* key, KV* kv) {

}

void hash_table_dump_kv(HashTable *ht, KV* kv) {

}

void hash_table_dump(HashTable *ht) {

}

void hash_table_cleanup(HashTable* ht) {
    free(ht->table);
}
