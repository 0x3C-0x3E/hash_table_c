#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

uint32_t str_hash(uint8_t *buffer, size_t buffer_size);

#define INIT_TABLE_CAP 255

#define HASH_TABLE_MAX 255 * 255

typedef struct KeyValuePair {
    uint8_t *key;
    size_t key_size;
    uint32_t value;

    bool occupied;
} KV;

typedef struct HashTable {
    KV *table;
    size_t count;
    size_t capacity;

} HashTable;

int hash_table_init(HashTable* ht);

// 0 -> false
// 1 -> true
//-1 -> index out of range
int hash_table_is_occupied(const HashTable *ht, size_t index);

int hash_table_insert(HashTable *ht, const KV *kv);

// touch meaning creates the entry if it does not exist
// and if it does, inc value by 1
int hash_table_touch(HashTable *ht, size_t index);

void hash_table_dump(HashTable *ht);

void hash_table_cleanup(HashTable* ht);
