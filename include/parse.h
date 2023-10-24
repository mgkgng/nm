#pragma once

#include <stdint.h>
#include <elf.h>
#include <stdio.h>

typedef struct {
    unsigned char arch;
    unsigned char encoding;
} elf_prop_t;

void parse(const uint8_t *data, elf_prop_t *prop);