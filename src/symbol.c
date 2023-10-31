#include "symbol.h"

#include <stdio.h>

// void print_section_data_64(Elf64_Shdr *curr_section, char *sh_string_table) {
//     printf("==========================\n");
//     printf("section name: %s\n", sh_string_table + curr_section->sh_name);
//     printf("section type: %ld\n", curr_section->sh_type);
//     printf("section flags: %ld\n", curr_section->sh_flags);
//     printf("section addr: %ld\n", curr_section->sh_addr);
//     printf("section offset: %ld\n", curr_section->sh_offset);
//     printf("section size: %ld\n", curr_section->sh_size);
//     printf("section link: %ld\n", curr_section->sh_link);
//     printf("section info: %ld\n", curr_section->sh_info);
//     printf("section addralign: %ld\n", curr_section->sh_addralign);
//     printf("section entsize: %ld\n", curr_section->sh_entsize);
// }

static void process_symbol_table_32(void *data, Elf32_Shdr *symtab, char *string_table, elf_prop_t *prop, t_list **symbol_data) {
    // Calculate the number of symbols
    int num_symbols = symtab->sh_size / symtab->sh_entsize;

    // Pointer to the array of symbols
    Elf32_Sym *symbols = (Elf32_Sym *)((char *)data + symtab->sh_offset);

    for (int i = 0; i < num_symbols; i++) {
        Elf32_Sym *sym = &symbols[i];

        symbol_t *symbol = malloc(sizeof(symbol_t));
        symbol->name = string_table + sym->st_name;
        symbol->addr_32 = sym->st_value;
        symbol->type = ELF32_ST_TYPE(sym->st_info);
        symbol->bind = ELF32_ST_BIND(sym->st_info);
        symbol->visibility = ELF32_ST_VISIBILITY(sym->st_other);
        symbol->shndx = sym->st_shndx;

        if (symbol->name[0] == '\0' && symbol->shndx != SHN_UNDEF && symbol->shndx != SHN_ABS) {
            Elf32_Shdr *curr_section = prop->section_header + symbol->shndx * prop->section_entry_size;
            char *section_name = prop->shstrtab + curr_section->sh_name;
            symbol->name = section_name;
        }

        ft_lstadd_back(symbol_data, ft_lstnew(symbol));
    }
}

static void process_symbol_table_64(void *data, Elf64_Shdr *symtab, char *string_table, elf_prop_t *prop, t_list **symbol_data) {
    // Calculate the number of symbols
    int num_symbols = symtab->sh_size / symtab->sh_entsize;

    // Pointer to the array of symbols
    Elf64_Sym *symbols = (Elf64_Sym *)((char *)data + symtab->sh_offset);

    for (int i = 0; i < num_symbols; i++) {
        Elf64_Sym *sym = &symbols[i];

        symbol_t *symbol = malloc(sizeof(symbol_t));
        symbol->name = string_table + sym->st_name;
        symbol->addr_64 = sym->st_value;
        symbol->type = ELF64_ST_TYPE(sym->st_info);
        symbol->bind = ELF64_ST_BIND(sym->st_info);
        symbol->visibility = ELF64_ST_VISIBILITY(sym->st_other);
        symbol->shndx = sym->st_shndx;

        if (symbol->name[0] == '\0' && symbol->shndx != SHN_UNDEF && symbol->shndx != SHN_ABS && symbol->shndx != SHN_COMMON) {
            Elf64_Shdr *curr_section = prop->section_header + symbol->shndx * prop->section_entry_size;
            char *section_name = prop->shstrtab + curr_section->sh_name;
            symbol->name = section_name;
        }

        ft_lstadd_back(symbol_data, ft_lstnew(symbol));
    }
}

t_list *extract_symbol_data_32(void *data, elf_prop_t *prop) {
    Elf32_Shdr *shdr_start = (Elf32_Shdr *) prop->section_header;

    // Some value adjustments (ref. man elf)
    if (prop->section_entry_nb == 0)
        prop->section_entry_nb = shdr_start->sh_size;
    if (prop->string_table_index == SHN_LORESERVE)
        prop->string_table_index = shdr_start->sh_link;
    
    // Locate the section header string table
    Elf32_Shdr *shstr_section = &shdr_start[prop->string_table_index];
    char *shstrtab = (char *) data + shstr_section->sh_offset;
    prop->shstrtab = shstrtab;

    // Iterate over the section headers to locate the string table and symbol tables
    Elf32_Shdr *string_table = NULL;
    Elf32_Shdr *symbol_table = NULL;
    t_list *symbol_data = NULL;
    for (int i = 0; i < prop->section_entry_nb; i++) {
        Elf32_Shdr *curr_section = (Elf32_Shdr *)((char *)shdr_start + (i * prop->section_entry_size));

        // Retrieve the section name from the section header string table
        char *section_name = shstrtab + curr_section->sh_name;

        // Check if the current section is the string table or the symbol table
        if (ft_strcmp(section_name, ".strtab") == 0)
            string_table = curr_section;
        else if (ft_strcmp(section_name, ".symtab") == 0)
            symbol_table = curr_section;
    }

    if (symbol_table)
        process_symbol_table_32(data, symbol_table, (char *) data + string_table->sh_offset, prop, &symbol_data);

    return symbol_data;

}

t_list *extract_symbol_data_64(void *data, elf_prop_t *prop) {
    Elf64_Shdr *shdr_start = (Elf64_Shdr *) prop->section_header;

    // Some value adjustments (ref. man elf)
    if (prop->section_entry_nb == 0)
        prop->section_entry_nb = shdr_start->sh_size;
    if (prop->string_table_index == SHN_LORESERVE)
        prop->string_table_index = shdr_start->sh_link;
    
    // Locate the section header string table
    Elf64_Shdr *shstr_section = &shdr_start[prop->string_table_index];
    char *shstrtab = (char *) data + shstr_section->sh_offset;
    prop->shstrtab = shstrtab;

    // Iterate over the section headers to locate the string table and symbol tables
    Elf64_Shdr *string_table = NULL;
    Elf64_Shdr *symbol_table = NULL;
    t_list *symbol_data = NULL;
    for (int i = 0; i < prop->section_entry_nb; i++) {
        Elf64_Shdr *curr_section = (Elf64_Shdr *)((char *)shdr_start + (i * prop->section_entry_size));

        // Retrieve the section name from the section header string table
        char *section_name = shstrtab + curr_section->sh_name;

        // Check if the current section is the string table or the symbol table
        if (ft_strcmp(section_name, ".strtab") == 0)
            string_table = curr_section;
        else if (ft_strcmp(section_name, ".symtab") == 0)
            symbol_table = curr_section;
    }

    if (symbol_table)
        process_symbol_table_64(data, symbol_table, (char *) data + string_table->sh_offset, prop, &symbol_data);

    return symbol_data;
}