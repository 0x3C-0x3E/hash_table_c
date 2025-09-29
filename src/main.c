#include "tokenizer.h"

Tokenizer tk;

int main(void) {
    tokenizer_init(&tk);

    tokenizer_read_file("res/file_very_smol.txt", &tk);
    tokenizer_parse_file(&tk);

    tokenizer_dump(&tk);

    tokenizer_cleanup(&tk);
    return 0;
}
