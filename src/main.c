#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "parse.h"

void *read_binary(char *path, size_t *length) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("error here\n");
        return NULL;
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        printf("error here\n");
        close(fd);
        return NULL;
    }
    *length = statbuf.st_size;

    void *addr = mmap(NULL, *length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "Error mmapping the file: %s\n", strerror(errno));
        close(fd);
        return NULL;
    }

    close(fd);
    return addr;
}

void process_symbol_table32(void *file_map, Elf32_Shdr *symtab, Elf32_Shdr *strtab, char *string_table) {

}

void process_symbol_table64(void *data, Elf64_Shdr *symtab, Elf64_Shdr *strtab, char *string_table) {
    // Calculate the number of symbols
    int num_symbols = symtab->sh_size / symtab->sh_entsize;

    // Pointer to the array of symbols
    Elf64_Sym *symbols = (Elf64_Sym *)((char *)data + symtab->sh_offset);

    for (int i = 0; i < num_symbols; i++) {
        Elf64_Sym *sym = &symbols[i];

        // Get the name of the symbol
        char *name = string_table + sym->st_name;

        // Print the symbol's value and name
        // For simplicity, we print the value as is, without considering special types of symbols.
        printf("%016lx %s\n", (unsigned long)sym->st_value, name);
    }
}


int main(int argc, char **argv) {
    (void) argc;
    
    size_t len;
    void *addr = read_binary(argv[1], &len);
    if (!addr)
        return 1;
    
    elf_prop_t prop;
    void *res = parse(addr, &prop);
    if (!res) {
        munmap(addr, len);
        return 1;
    }

    if (prop.arch == ELFCLASS32) {
        printf("32-bit ELF file\n");
    } else if (prop.arch == ELFCLASS64) {
        printf("64-bit ELF file\n");
    } else {
        printf("invalid class\n");
        return 1;
    }


    munmap(addr, len);
    return 0;
}