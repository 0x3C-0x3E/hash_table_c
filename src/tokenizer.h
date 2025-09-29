#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define INIT_SP_CAP 1024
#define INIT_TK_CAP 255

extern const char stop_chars[];
extern size_t stop_chars_count;

typedef struct Token {
    const uint8_t *string_ptr;
    size_t string_size;
} Token;

typedef struct Tokenizer {
    FILE *file_ptr;

    uint8_t *string_pool;
    size_t pool_count;
    size_t pool_capacity;

    size_t next_char_ptr;

    Token *tokens;
    size_t tokens_count;
    size_t tokens_capacity;

} Tokenizer;

int tokenizer_init(Tokenizer *tk);

int tokenizer_read_file(const char *file_name, Tokenizer *tk);

void tokenizer_parse_file(Tokenizer *tk);

void tokenizer_cleanup(Tokenizer *tk);
