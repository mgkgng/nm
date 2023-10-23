#include <elf.h>
#include <stdio.h>

#include "parse.h"

static int check_magic_code(const uint8_t *data) {
    return (data[0] == 0x7f && data[1] == 'E' && data[2] == 'L' && data[3] == 'F');
}

void parse_header(const uint8_t *data, elf_prop_t *prop) {
    if (!check_magic_code(data)) {
        printf("magic code wrong\n");
        return;
    }

    if (data[4] == ELFCLASS32) {
        printf("32 bits architecture\n");
    } else if (data[4] == ELFCLASS64) {
        printf("64 bits architecture\n");
    }

    if (data[5] == ELFDATA2LSB) {
        printf("two's complement, little-endian\n");
    } else if (data[5] == ELFDATA2MSB) {
        printf("two's complement, big-endian\n");
    }
    
    printf("gut\n");
}