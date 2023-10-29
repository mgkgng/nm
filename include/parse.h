#pragma once

#include <stdint.h>

#include "elf_prop.h"
#include "libft.h"

#define OPT_A 1 << 0
#define OPT_G 1 << 1
#define OPT_U 1 << 2
#define OPT_R 1 << 3
#define OPT_P 1 << 4

extern uint8_t g_opts;
extern uint8_t g_file_nb;

t_list *parse(int ac, char **av);