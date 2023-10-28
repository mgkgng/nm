#include "parse.h"

// typedef struct {
//     unsigned char e_ident[EI_NIDENT];    /* Magic number and other info */
//     uint16_t      e_type;                /* Object file type */
//     uint16_t      e_machine;             /* Architecture */
//     uint32_t      e_version;             /* Object file version */
//     ElfN_Addr     e_entry;               /* Entry point virtual address */
//     ElfN_Off      e_phoff;               /* Program header table file offset */
//     ElfN_Off      e_shoff;               /* Section header table file offset */
//     uint32_t      e_flags;               /* Processor-specific flags */
//     uint16_t      e_ehsize;              /* ELF header size in bytes */
//     uint16_t      e_phentsize;           /* Program header table entry size */
//     uint16_t      e_phnum;               /* Program header table entry count */
//     uint16_t      e_shentsize;           /* Section header table entry size */
//     uint16_t      e_shnum;               /* Section header table entry count */
//     uint16_t      e_shstrndx;            /* Section header string table index */
// } ElfN_Ehdr;

static void *process_elf_header32(const uint8_t *data, elf_prop_t *prop) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
    prop->section_header = (void *) data + ehdr->e_shoff;
    prop->section_entry_nb = ehdr->e_shnum;
    prop->section_entry_size = ehdr->e_shentsize;
    if (ehdr->e_shstrndx == SHN_UNDEF) {
        printf("no section header string table\n");
        return NULL;
    }
    prop->string_table_index = ehdr->e_shstrndx;
}

static void *process_elf_header64(const uint8_t *data, elf_prop_t *prop) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) data;
    prop->section_header = (void *) data + ehdr->e_shoff;
    prop->section_entry_nb = ehdr->e_shnum; // TODO check if section size in section header is 0
    prop->section_entry_size = ehdr->e_shentsize;
    if (ehdr->e_shstrndx == SHN_UNDEF) {
        printf("no section header string table\n");
        return NULL;
    }
    prop->string_table_index = ehdr->e_shstrndx;
}

void *parse(const uint8_t *data, elf_prop_t *prop) {
    // Check if magic code is correct (ELF format)
    // Magic code: 0x7f, 'E', 'L', 'F'
    if (!(data[0] == EI_MAG0 && data[1] == EI_MAG1 && data[2] == EI_MAG2 && data[3] == EI_MAG3)) {
        printf("magic code wrong\n");
        return;
    }

    if (data[4] == ELFCLASSNONE) {
        printf("invalid class\n");
        return NULL;
    }

    // save arch and encoding information which will be essential for further parsing
    prop->arch = data[4];
    prop->encoding = data[5];

    void *res = (data[4] == ELFCLASS32) ? process_elf_header32(data, prop) : process_elf_header64(data, prop);
    if (!res) {
        printf("error processing ELF header\n");
        return NULL;
    }
}