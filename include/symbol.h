#pragma once

#include <elf.h>
#include <stdint.h>

#include "libft.h"

typedef struct {
    char *name;
    uint64_t value; // address
    unsigned char type;
    unsigned char bind;
} symbol_t;

t_list *extract_symbol_data_32(void *data, Elf32_Shdr *shdr, uint16_t string_table_index, uint16_t section_entry_nb);
t_list *extract_symbol_data_64(void *data, Elf64_Shdr *shdr, uint16_t string_table_index, uint16_t section_entry_nb);
