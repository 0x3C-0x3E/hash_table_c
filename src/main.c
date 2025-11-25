#include "tokenizer/tokenizer.h"
#include "hash_table/hash_table.h"

Tokenizer tk;
HashTable ht;

int main(void) {
    tokenizer_init(&tk);
    hash_table_init(&ht);

    tokenizer_read_file("res/file_very_smol.txt", &tk);
    tokenizer_parse_file(&tk);

    // tokenizer_dump(&tk);
    
    for (size_t i = 0; i < tk.tokens_count; ++i) {
        Key key = (Key) {
            .key = (uint8_t*) (tk.string_pool + tk.tokens[i].string_offset),
            .key_size = tk.tokens[i].string_size,
        };
        Value curr_value = 1;
        int ret;
        if (hash_table_get(&ht, &key, &curr_value) == HT_NOT_IN_TABLE) {
            ht_return ret = hash_table_insert(&ht, &key, &curr_value);
            if (ret == HT_TABLE_OVERFLOW) {
                return 1;
            }
            continue;
        }

        curr_value += 1;
        hash_table_update(&ht, &key, &curr_value);
    }

    hash_table_dump(&ht);

    hash_table_cleanup(&ht);
    tokenizer_cleanup(&tk);
    return 0;
}
