#include "display.h"
#include <stdio.h>

int compare_symbol(void *a, void *b) {
    symbol_t *sym_a = a;
    symbol_t *sym_b = b;
    return ft_strcmp(sym_a->name, sym_b->name);
}

unsigned char get_symbol_type_char(symbol_t *sym, elf_prop_t *prop) {
    char type_char = '?'; // Default is unknown

    // Check for undefined symbol
    if (sym->shndx == SHN_UNDEF) {
        type_char = 'U';
    } else if (sym->shndx == SHN_ABS) { // Check for absolute symbol
        type_char = 'A';
    } else {
        // Determine symbol type based on its section header
        Elf64_Shdr section_header = ((Elf64_Shdr *) (prop->section_header))[sym->shndx];
        switch (section_header.sh_type) {
            case SHT_PROGBITS:
                if (section_header.sh_flags & SHF_ALLOC) {
                    if (section_header.sh_flags & SHF_EXECINSTR) {
                        type_char = 'T'; // code section
                    } else if (section_header.sh_flags & SHF_WRITE) {
                        type_char = 'D'; // Initialized data section
                    } else {
                        type_char = 'R'; // Read-only data section
                    }
                }
                break;

            case SHT_NOBITS:
                if (section_header.sh_flags & SHF_ALLOC) {
                    if (section_header.sh_flags & SHF_WRITE) {
                        type_char = 'B'; // Uninitialized data section (BSS)
                    }
                }
                break;

            case SHT_DYNAMIC:
                type_char = 'D'; // Dynamic linking information
                break;



            default:
                break; // Keep as '?' for unknown/other
        }
    }

    // Handle special STT (Symbol Table Type) values
    switch (sym->type) {
        case STT_OBJECT:
            if (type_char == 'U') {
                type_char = 'C'; // Common symbols (uninitialized data)
            } else {
                type_char = 'D'; // Data symbols
            }
            break;

        case STT_FUNC:
            type_char = 'T'; // Functions are in the text section
            break;

        case STT_SECTION:
            type_char = 'S'; // These symbols are associated with a section
            break;

        case STT_FILE:
            type_char = 'F'; // Source file associated with the object file
            break;

        case STT_COMMON:
            type_char = 'C'; // Common symbols (uninitialized data)
            break;

        case STT_TLS:
            type_char = 'T'; // Thread-local storage
            break;

        case STT_GNU_IFUNC:
            type_char = 'i'; // GNU Indirect Function
            break;

        default:
            break;
    }

    // Handle weak symbols (these could override previous decisions except 'U')
    if (sym->bind == STB_WEAK) {
        type_char = (sym->shndx == SHN_UNDEF) ? 'w' : 'W';
    }

    // For unique global symbols (GNU extension), use 'u'
    if (sym->bind == STB_GNU_UNIQUE) {
        type_char = 'u';
    }

    // If the symbol is local, convert type character to lowercase
    if (sym->bind == STB_LOCAL) {
        type_char = tolower(type_char);
    }

    return type_char;
}

void display_symbol_data(t_list *symbol_data, elf_prop_t *prop) {
    // Sort the symbol data
    if (g_opts & OPT_P)
        ;
    else if (g_opts & OPT_R)
        ft_lstsort(&symbol_data, compare_symbol, SORT_REVERSE);
    else
        ft_lstsort(&symbol_data, compare_symbol, SORT_REVERSE);

    while (symbol_data) {
        symbol_t *symbol = symbol_data->content;
        if (symbol->bind == STB_LOCAL && !(g_opts & OPT_A)) {
            symbol_data = symbol_data->next;
            continue;
        }
        if (g_opts & OPT_G && symbol->bind != STB_GLOBAL) {
            symbol_data = symbol_data->next;
            continue;
        }
        if (g_opts & OPT_U && symbol->shndx != SHN_UNDEF) {
            symbol_data = symbol_data->next;
            continue;
        }
        printf("%016lx %c %s\n", symbol->value, get_symbol_type_char(symbol, prop), symbol->name);
        symbol_data = symbol_data->next;
    }
}