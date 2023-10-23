#pragma once

#include <stdint.h>

typedef struct {
    int is_32bit;
    int is_little_endian;
} elf_prop_t;

void parse_header(const uint8_t *data, elf_prop_t *prop);