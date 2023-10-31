#include "display.h"
#include <stdio.h>

int compare_symbol(void *a, void *b) {
    symbol_t *sym_a = a;
    symbol_t *sym_b = b;
    return ft_strcmp(sym_a->name, sym_b->name);
}

static void write_hex_32(uint32_t value) {
    char buffer[9];
    const char hex[] = "0123456789abcdef";

    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex[value & 0xF];
        value >>= 4;
    }
    buffer[8] = '\0';
    write(STDOUT_FILENO, buffer, 8);
}

static void write_hex_64(uint64_t value) {
    char buffer[17];
    const char hex[] = "0123456789abcdef";

    for (int i = 15; i >= 0; i--) {
        buffer[i] = hex[value & 0xF];
        value >>= 4;
    }
    buffer[16] = '\0';
    write(STDOUT_FILENO, buffer, 16);
}

static void display_addr_32(uint32_t value, unsigned char shndx) {
    if (shndx == SHN_UNDEF)
        write(STDOUT_FILENO, "        ", 8);
    else
        write_hex_32(value);
    write(STDOUT_FILENO, " ", 1);
}

static void display_addr_64(uint64_t value, unsigned char shndx) {
    if (shndx == SHN_UNDEF)
        write(STDOUT_FILENO, "                ", 16);
    else
        write_hex_64(value);
    write(STDOUT_FILENO, " ", 1);

}

static unsigned char get_symbol_type_char_32(symbol_t *sym, elf_prop_t *prop) {
    if (!sym->addr_32 && sym->shndx == SHN_UNDEF && sym->bind == STB_LOCAL) {
        return 0;
    }
    if (sym->shndx == SHN_ABS)
        return (sym->bind == STB_LOCAL) ? 'a' : 'A';

    Elf32_Shdr *curr_section = prop->section_header + sym->shndx * prop->section_entry_size;
    char *section_name = prop->shstrtab + curr_section->sh_name;

    if (ft_strcmp(section_name, ".bss") == 0)
        return (sym->bind == STB_LOCAL) ? 'b' : 'B';
    else if (ft_strcmp(section_name, ".debug") == 0)
        return 'N';
    else if (ft_strcmp(section_name, ".sbss") == 0)
        return (sym->bind == STB_LOCAL) ? 's' : 'S';

    if (sym->shndx == SHN_ABS)
        return 'A';
    else if (sym->shndx == SHN_COMMON)
        return 'C';

    if (curr_section->sh_type == SHT_PROGBITS && curr_section->sh_flags & SHF_ALLOC) {
        if (curr_section->sh_flags & SHF_EXECINSTR)
            return (sym->bind == STB_LOCAL) ? 't' : 'T';
        else if (curr_section->sh_flags & SHF_WRITE)
            return (sym->bind == STB_LOCAL) ? 'd' : 'D';
        else
            return (sym->bind == STB_LOCAL) ? 'r' : 'R';
    } else if (curr_section->sh_type == SHT_NOBITS && curr_section->sh_flags & SHF_ALLOC) {
        if (curr_section->sh_flags & SHF_WRITE)
            return (sym->bind == STB_LOCAL) ? 'b' : 'B';
        else
            return (sym->bind == STB_LOCAL) ? 'n' : 'N';
    } else if (curr_section->sh_type == SHT_DYNAMIC) {
        return (sym->bind == STB_LOCAL) ? 'd' : 'D';
    }

    if (sym->bind == STB_WEAK) {
        if (sym->type == STT_OBJECT)
            return (sym->shndx == SHN_UNDEF) ? 'v' : 'V';
        return (sym->shndx == SHN_UNDEF) ? 'w' : 'W';
    } else if (sym->bind == STB_GNU_UNIQUE) {
        return 'u';
    }

    if (sym->type == STT_GNU_IFUNC)
        return 'i';

    if (sym->shndx == SHN_UNDEF)
        return 'U';
    
    if (ft_strcmp(section_name, ".data") == 0)
        return (sym->bind == STB_LOCAL) ? 'd' : 'D';
    if (ft_strcmp(section_name, ".rodata"))
        return (sym->bind == STB_LOCAL) ? 'r' : 'R';
    
    return '?';
}

static unsigned char get_symbol_type_char_64(symbol_t *sym, elf_prop_t *prop) {
    if (!sym->addr_64 && sym->shndx == SHN_UNDEF && sym->bind == STB_LOCAL)
        return 0;

    if (sym->shndx == SHN_ABS)
        return (sym->bind == STB_LOCAL) ? 'a' : 'A';

    Elf64_Shdr *curr_section = prop->section_header + sym->shndx * prop->section_entry_size;
    char *section_name = prop->shstrtab + curr_section->sh_name;

    if (ft_strcmp(section_name, ".bss") == 0)
        return (sym->bind == STB_LOCAL) ? 'b' : 'B';
    else if (ft_strcmp(section_name, ".debug") == 0)
        return 'N';
    else if (ft_strcmp(section_name, ".sbss") == 0)
        return (sym->bind == STB_LOCAL) ? 's' : 'S';

    if (sym->shndx == SHN_ABS)
        return 'A';
    else if (sym->shndx == SHN_COMMON)
        return 'C';

    if (curr_section->sh_type == SHT_PROGBITS && curr_section->sh_flags & SHF_ALLOC) {
        if (curr_section->sh_flags & SHF_EXECINSTR)
            return (sym->bind == STB_LOCAL) ? 't' : 'T';
        else if (curr_section->sh_flags & SHF_WRITE)
            return (sym->bind == STB_LOCAL) ? 'd' : 'D';
        else
            return (sym->bind == STB_LOCAL) ? 'r' : 'R';
    } else if (curr_section->sh_type == SHT_NOBITS && curr_section->sh_flags & SHF_ALLOC) {
        if (curr_section->sh_flags & SHF_WRITE)
            return (sym->bind == STB_LOCAL) ? 'b' : 'B';
        else
            return (sym->bind == STB_LOCAL) ? 'n' : 'N';
    } else if (curr_section->sh_type == SHT_DYNAMIC) {
        return (sym->bind == STB_LOCAL) ? 'd' : 'D';
    }

    if (sym->bind == STB_WEAK) {
        if (sym->type == STT_OBJECT)
            return (sym->shndx == SHN_UNDEF) ? 'v' : 'V';
        return (sym->shndx == SHN_UNDEF) ? 'w' : 'W';
    } else if (sym->bind == STB_GNU_UNIQUE) {
        return 'u';
    }

    if (sym->type == STT_GNU_IFUNC)
        return 'i';

    if (sym->shndx == SHN_UNDEF)
        return 'U';
    
    if (ft_strcmp(section_name, ".data") == 0)
        return (sym->bind == STB_LOCAL) ? 'd' : 'D';
    if (ft_strcmp(section_name, ".rodata"))
        return (sym->bind == STB_LOCAL) ? 'r' : 'R';
    
    return '?';
}

void display_symbol_data(t_list *symbol_data, elf_prop_t *prop) {
    // Sort the symbol data + Apply sort options
    // -r : reverse sort
    // -p : don't sort
    if (!(g_opts & OPT_P))
        ft_lstsort(&symbol_data, compare_symbol, (g_opts & OPT_R) ? SORT_REVERSE : SORT_DEFAULT);

    // Get the function pointer according to the architecture
    unsigned char (*get_symbol_type_char)(symbol_t *, elf_prop_t *) = (prop->arch == ELFCLASS32) ? get_symbol_type_char_32 : get_symbol_type_char_64;

    while (symbol_data) {
        symbol_t *symbol = symbol_data->content;

        // Apply options
        // -a : display all symbols
        // -g : display only global symbols
        // -u : display only undefined symbols
        if (!(g_opts & OPT_A) && (symbol->type == STT_FILE || symbol->type == STT_SECTION)) {
            symbol_data = symbol_data->next;
            continue;
        }
        if (g_opts & OPT_G && symbol->bind == STB_LOCAL) {
            symbol_data = symbol_data->next;
            continue;
        }
        if (g_opts & OPT_U && symbol->shndx != SHN_UNDEF) {
            symbol_data = symbol_data->next;
            continue;
        }

        // Get the symbol type character
        unsigned char symbol_char = get_symbol_type_char(symbol, prop);
        if (!symbol_char) {
            symbol_data = symbol_data->next;
            continue;
        }

        // Display the symbol data in the correct format
        if (prop->arch == ELFCLASS32)
            display_addr_32(symbol->addr_32, symbol->shndx);
        else
            display_addr_64(symbol->addr_64, symbol->shndx);
        ft_printf("%c %s\n", symbol_char, symbol->name);

        // Next symbol
        symbol_data = symbol_data->next;
    }
}