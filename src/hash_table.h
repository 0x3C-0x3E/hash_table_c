#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

uint32_t hash(uint8_t *buffer, size_t buffer_size);

#define INIT_TABLE_CAP 255

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

bool hash_table_is_occupied(const HashTable *ht, size_t index);

int hash_table_insert(HashTable *ht, size_t index, const KV *kv);

void hash_table_cleanup(HashTable* ht);
