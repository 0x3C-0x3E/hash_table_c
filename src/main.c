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
    
    for (size_t i = 0; i < 10; ++i) {

        KV kv = {
            .key = (uint8_t*) (tk.string_pool + tk.tokens[i].string_offset),
            .key_size = tk.tokens[i].string_size,
            .occupied = true,
            .value = 1,
        };

        hash_table_insert(&ht, &kv);
    }

    hash_table_dump(&ht);

    hash_table_cleanup(&ht);
    tokenizer_cleanup(&tk);
    return 0;
}
