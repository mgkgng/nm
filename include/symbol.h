#pragma once

#include <elf.h>

#include "libft.h"
#include "elf_prop.h"

typedef struct {
    char *name;
    unsigned char type;
    unsigned char bind;
    unsigned char visibility;
    uint16_t shndx;
    Elf32_Addr addr_32;
    Elf64_Addr addr_64;
} symbol_t;

t_list *extract_symbol_data_32(void *data, elf_prop_t *prop);
t_list *extract_symbol_data_64(void *data, elf_prop_t *prop);
