#pragma once

#include <elf.h>

#include "libft.h"
#include "elf_prop.h"

typedef struct {
    char *name;
    uint64_t value_64;
    uint32_t value_32;
    unsigned char type;
    unsigned char bind;
    unsigned char visibility;
    uint16_t shndx;
} symbol_t;

t_list *extract_symbol_data_32(void *data, elf_prop_t *prop);
t_list *extract_symbol_data_64(void *data, elf_prop_t *prop);
