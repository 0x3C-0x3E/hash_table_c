#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "hash.h"

#define INIT_TABLE_CAP 255
#define HASH_TABLE_MAX 255 * 255

typedef struct Key {
    uint8_t *key;
    size_t key_size;
} Key;

typedef struct KeyValuePair {
    Key key;
    uint32_t value;

    bool occupied;
} KV;

typedef struct HashTable {
    KV *table;
    size_t count;
    size_t capacity;
} HashTable;

typedef enum {
    HT_SUCESS = 0,
    HT_NOT_IN_TABLE,
    HT_INDEX_OUT_OF_BOUNDS,
} ht_return;

typedef enum {
    HT_INSERT = 0, // insert or overwrite regardless if present
    HT_INSERT_UPDATE = 1, // only update if present
    HT_INSERT_APPEND = 2, // only append if not present
} ht_insertion_flag;

int hash_table_init(HashTable* ht);

int hash_table_insert(HashTable *ht, const KV *kv);

int hash_table_update(HashTable *ht, const KV *kv);

int hash_table_append(HashTable *ht, const KV *kv);

int hash_table_get(HashTable *ht, const Key *key, KV *output_kv);

size_t hash_table_get_kv_index(HashTable *ht, KV *kv);

void hash_table_dump_kv(HashTable *ht, KV* kv);

int hash_table_set(HashTable *ht, const KV *kv);

void hash_table_dump(HashTable *ht);

void hash_table_cleanup(HashTable* ht);
