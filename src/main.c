#include "tokenizer.h"
#include "hash_table.h"

Tokenizer tk;
HashTable ht;

int main(void) {
    tokenizer_init(&tk);
    hash_table_init(&ht);

    tokenizer_read_file("res/file_very_smol.txt", &tk);
    tokenizer_parse_file(&tk);

    // tokenizer_dump(&tk);
    
    char c[] = "Hi";
    KV kv = {
        .key = (uint8_t*)&c,
        .key_size = 2,
        .occupied = true,
        .value = 1,
    };

    hash_table_insert(&ht, 0, &kv);

    tokenizer_cleanup(&tk);
    return 0;
}
