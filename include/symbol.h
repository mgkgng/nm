#pragma once

#include <elf.h>

#include "libft.h"
#include "elf_prop.h"

typedef struct {
    char *name;
    uint64_t value; // address
    unsigned char type;
    unsigned char bind;
} symbol_t;

t_list *extract_symbol_data_32(void *data, elf_prop_t *prop);
t_list *extract_symbol_data_64(void *data, elf_prop_t *prop);
