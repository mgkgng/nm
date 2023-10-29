#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <elf.h>

#include "libft.h"
#include "symbol.h"
#include "elf_prop.h"

int run_nm(const char* path);