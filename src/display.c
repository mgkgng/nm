#include "display.h"
#include <stdio.h>

unsigned char get_symbol_type_char(symbol_t *sym, elf_prop_t *prop) {
    char type_char = '?'; // Default is unknown

    // Check for undefined symbol
    if (sym->shndx == SHN_UNDEF) {
        type_char = 'U';
    } else if (sym->shndx == SHN_ABS) {
        // Check for absolute symbol
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

            // Other specific section types can be handled here as needed

            default:
                break; // Keep as '?' for unknown/other
        }
    }

    // Handle special STT (Symbol Table Type) values
    switch (sym->type) {
        case STT_OBJECT:
            if (type_char == '?') {
                type_char = 'D'; // Typically, these are data objects
            }
            break;

        case STT_FUNC:
            type_char = 'T'; // Functions are in the text section
            break;

        case STT_SECTION:
        case STT_FILE:
            // We can choose to implement specific logic for these if desired
            break;

        case STT_COMMON:
            type_char = 'C'; // Common symbols (uninitialized data)
            break;

        case STT_TLS:
            type_char = 'T'; // Thread-local storage
            break;

        // Add other cases as needed

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
    while (symbol_data) {
        symbol_t *symbol = symbol_data->content;
        if (symbol->bind == STB_LOCAL && !(g_opts & OPT_A)) {
            symbol_data = symbol_data->next;
            continue;
        }
        printf("%016lx %c %s\n", symbol->value, get_symbol_type_char(symbol, prop), symbol->name);
        symbol_data = symbol_data->next;
    }
}