// #include "parse.h"

// void process_symbol_table32(void *data, Elf32_Shdr *symtab, Elf32_Shdr *strtab, char *string_table) {

// }

// void process_symbol_table64(void *data, Elf64_Shdr *symtab, Elf64_Shdr *strtab, char *string_table) {
//     // Calculate the number of symbols
//     int num_symbols = symtab->sh_size / symtab->sh_entsize;

//     // Pointer to the array of symbols
//     Elf64_Sym *symbols = (Elf64_Sym *)((char *)data + symtab->sh_offset);

//     for (int i = 0; i < num_symbols; i++) {
//         Elf64_Sym *sym = &symbols[i];

//         // Get the name of the symbol
//         char *name = string_table + sym->st_name;

//         // Print the symbol's value and name
//         // For simplicity, we print the value as is, without considering special types of symbols.
//         ft_printf("%016lx %s\n", (unsigned long)sym->st_value, name);
//     }
// }

// void display32(void *data, elf_prop_t *prop) {
//     Elf32_Shdr *shdr = (Elf32_Shdr *) prop->section_header;
//     Elf32_Shdr *strtab = &shdr[prop->string_table_index];
// }

// void display64(void *data, elf_prop_t *prop) {
//     Elf64_Shdr *shdr = (Elf64_Shdr *) prop->section_header;
//     Elf64_Shdr *strtab = &shdr[prop->string_table_index];

//     // Get the string table
//     char *string_table = (char *)data + strtab->sh_offset;

//     // Iterate over the section headers
//     for (int i = 0; i < prop->section_entry_nb; i++) {
//         Elf64_Shdr *section = &shdr[i];

//         // Get the name of the section
//         char *name = string_table + section->sh_name;

//         // Print the name of the section

//         // Check if the section is a symbol table
//         if (section->sh_type == SHT_SYMTAB) {
//             ft_printf("%s\n", name);
//             process_symbol_table64(data, section, strtab, string_table);
//         }
//     }


// }