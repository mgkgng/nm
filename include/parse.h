#pragma once

#include <stdint.h>
#include <elf.h>
#include <stdio.h>


typedef struct {
    uint8_t arch;
    uint8_t encoding;
    void *section_header;
    uint16_t section_entry_nb;
    uint16_t section_entry_size;
    uint16_t string_table_index;
    
} elf_prop_t;

void *parse(const uint8_t *data, elf_prop_t *prop);