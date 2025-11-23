#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "hash.h"

#define HT_MAX_CAP 255 * 255
#define HT_INIT_CAP 255

typedef struct Key {
    uint8_t* key;
    size_t key_size;
} Key;

typedef uint32_t Value;

typedef struct KeyValuePair {
    Key key;
    Value value;

    bool occupied;
} KV;

typedef struct HashTable {
    KV* table;
    size_t count;
    size_t capacity;
} HashTable;

typedef enum {
    HT_SUCESS = 0,
    HT_ERROR = 1,
    HT_NOT_IN_TABLE = 2,
    HT_TABLE_OVERFLOW = 3,
} ht_return;

ht_return hash_table_init(HashTable* ht);

ht_return hash_table_insert(HashTable* ht, const KV* kv);

ht_return hash_table_update(HashTable* ht, const KV* kv);

ht_return hash_table_append(HashTable* ht, const KV* kv);

ht_return hash_table_get(HashTable* ht, const Key* key, KV* kv);

void hash_table_dump_kv(HashTable *ht, KV* kv);

void hash_table_dump(HashTable *ht);

void hash_table_cleanup(HashTable* ht);

