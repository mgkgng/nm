

#include "parse.h"

static int check_magic_code(const uint8_t *data) {
    return (data[0] == 0x7f && data[1] == 'E' && data[2] == 'L' && data[3] == 'F');
}

void *parse_elf_header(const uint8_t *data, elf_prop_t *prop) {
    // Check if magic code is correct (ELF format)
    if (!check_magic_code(data)) {
        printf("magic code wrong\n");
        return;
    }

    // save arch and encoding information
    prop->arch = data[4];
    prop->encoding = data[5];

    // If unknown data format
    if (prop->encoding == ELFDATANONE) {
        printf("invalid data encoding\n");
        return NULL;
    }

    // 32 or 64 bit architecture, then parse the header,
    // then return the pointer to the program header
    // if not invalid class
    if (prop->arch == ELFCLASS32) {
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
        return data + ehdr->e_phoff;
    } else if (prop->arch == ELFCLASS64) {
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *) data;
        return data + ehdr->e_phoff;
    } else {
        printf("arch wrong\n");
        return NULL;
    }
}

void parse_program_header32(void *addr) {
    Elf32_Phdr *phdr = (Elf32_Phdr *) addr;
    printf("test32 %d\n", phdr->p_type);
}

void parse_program_header64(void *addr) {
    Elf64_Phdr *phdr = (Elf64_Phdr *) addr;
    printf("test64 %d\n", phdr->p_type);
}

void parse(const uint8_t *data, elf_prop_t *prop) {
    void *phdr_addr = parse_elf_header(data, prop);
    if (!phdr_addr)
        return;
    if (prop->arch == ELFCLASS32)
        parse_program_header32(phdr_addr);
    else
        parse_program_header64(phdr_addr);
}