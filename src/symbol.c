#include "symbol.h"

static void process_symbol_table32(void *data, Elf32_Shdr *symtab, Elf32_Shdr *strtab, char *string_table) {
    ft_printf("?\n");
}

static void process_symbol_table_64(void *data, Elf64_Shdr *symtab, Elf64_Shdr *strtab, char *string_table) {
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
        ft_printf("%016lx %s\n", (unsigned long)sym->st_value, name);
    }
}

t_list *extract_symbol_data_32(void *data, Elf32_Shdr *shdr, uint16_t string_table_index) {


}

t_list *extract_symbol_data_64(void *data, Elf64_Shdr *shdr, uint16_t string_table_index, uint16_t section_entry_nb) {
    // Locate the string table
    Elf64_Shdr *string_section = &shdr[string_table_index];
    char *string_table = (char *) data + string_section->sh_offset;

    // Iterate over the section headers
    for (int i = 0; i < section_entry_nb; i++) {
        Elf64_Shdr *section = &shdr[i];

        // Check if the section is a symbol table
        if (section->sh_type == SHT_SYMTAB) { // link editing
            ft_printf("here is a symbol table\n");
            process_symbol_table_64(data, section, string_section, string_table);
        } else if (section->sh_type == SHT_DYNSYM) { // Dynamic linking
            ft_printf("here is a dynamic symbol table\n");
            process_symbol_table_64(data, section, string_section, string_table);
        }
    }
}