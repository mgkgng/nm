#pragma once

#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <elf.h>

#include "libft.h"

typedef struct {
    uint8_t arch;
    uint8_t encoding;
    void *section_header;
    uint16_t section_entry_nb;
    uint16_t section_entry_size;
    uint16_t string_table_index;
} elf_prop_t;

int run_nm(const char* path);
void display_symbol_table_32(void *data, elf_prop_t *prop);
void display_symbol_table_64(void *data, elf_prop_t *prop);
