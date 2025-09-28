#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_TOKEN_SIZE 255

typedef struct Token {
    uint8_t string[MAX_TOKEN_SIZE];
    size_t token_size;
} Token;


