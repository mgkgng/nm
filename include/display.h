#pragma once

#include "symbol.h"
#include "elf_prop.h"
#include "parse.h"
#include "libft.h"

#define EMPTY_8 "        "
#define EMPTY_16 "                "

void display_symbol_data(t_list *symbol_data, elf_prop_t *prop);