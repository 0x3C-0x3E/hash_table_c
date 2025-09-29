#include "tokenizer.h"
#include <stdio.h>


const char stop_chars[] = {
    ' ',
    '.',
    ',',
    '?',
    '(',
    ')',
    ':',
    '"',
    '\n',
};

size_t stop_chars_count = sizeof(stop_chars) / sizeof(stop_chars[0]);

int tokenizer_init(Tokenizer *tk) {
    *tk = (Tokenizer) {
        .file_ptr = NULL,

        .pool_count = 0,
        .pool_capacity = INIT_SP_CAP,

        .next_char_ptr = 0,

        .tokens_count = 0,
        .tokens_capacity = INIT_TK_CAP,
    };
    
    tk->string_pool = malloc(sizeof(uint8_t) * tk->pool_capacity);
    tk->tokens = malloc(sizeof(Token) * tk->tokens_capacity);

    if (tk->string_pool == NULL || tk->tokens == NULL) {
        printf("[ERR] Malloc failed\n");
        return 1;
    }

    return 0;
}

int tokenizer_read_file(const char *file_name, Tokenizer *tk) {
    tk->file_ptr = fopen(file_name, "r");
    if (tk->file_ptr == NULL) {
        printf("[ERR] Could not read file [%s]\n", file_name);
        return 1;
    }

    return 0; 
}

static void tokenizer_create_new_token(Tokenizer *tk) {
    size_t chars_count = tk->next_char_ptr - tk->pool_count;

    if (chars_count == 0)
        return;
    
    if (tk->tokens_count >= tk->tokens_capacity) {
        tk->tokens_capacity *= 2;
        tk->tokens = realloc(tk->tokens, sizeof(Token) * tk->tokens_capacity);
        if (tk->tokens == NULL) {
            printf("[ERR] Realloc failed!\n");
            return;
        }
    }

    tk->tokens[tk->tokens_count] = (Token) {
        .string_offset = tk->pool_count,
        .string_size = chars_count,
    };
    

    tk->tokens_count ++;

    tk->pool_count += chars_count;
}

void tokenizer_parse_file(Tokenizer *tk) {
    int c;

    while ((c = fgetc(tk->file_ptr)) != EOF) {
        bool end_of_token = false;
        for (size_t i = 0; i < stop_chars_count; ++i) {
            if (c == stop_chars[i])
                end_of_token = true;
        }

        if (end_of_token) {
            tokenizer_create_new_token(tk);
            continue;
        }
        
        if (tk->next_char_ptr >= tk->pool_capacity) {
            tk->pool_capacity *= 2;
            tk->string_pool = realloc(tk->string_pool, sizeof(uint8_t) * tk->pool_capacity);
            if (tk->string_pool == NULL) {
                printf("[ERR] Realloc failed!\n");
                return;
            }
        }
        tk->string_pool[tk->next_char_ptr] = c;
        tk->next_char_ptr ++;
    }

    tokenizer_create_new_token(tk);

    fclose(tk->file_ptr);
}

void tokenizer_dump(Tokenizer *tk) {
    for (size_t i = 0; i < tk->tokens_count; ++i) {

        printf("[TOKEN] \n");
        printf("    Size: %zu\n", tk->tokens[i].string_size);
        printf("    Token: ");
        uint8_t *ptr = &tk->string_pool[tk->tokens[i].string_offset];

        for (size_t j = 0; j < tk->tokens[i].string_size; ++j) {
            putchar(ptr[j]);
        }
        printf("\n");
    }
}

void tokenizer_cleanup(Tokenizer *tk) {
    free(tk->tokens);
    free(tk->string_pool);
}
