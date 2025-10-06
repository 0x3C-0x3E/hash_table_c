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
    
    for (size_t i = 0; i < tk.tokens_count; ++i) {

        KV kv = {
            .key = (uint8_t*) (tk.string_pool + tk.tokens[i].string_offset),
            .key_size = tk.tokens[i].string_size,
            .occupied = true,
            .value = 1,
        };

        if (hash_table_insert(&ht, &kv)) {
            // already in table, added +1 to value
        }
    }

    // hash_table_dump(&ht);

    hash_table_cleanup(&ht);
    tokenizer_cleanup(&tk);
    return 0;
}
