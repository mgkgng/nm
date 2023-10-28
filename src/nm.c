#include "nm.h"

void *read_binary(const char *path, size_t *length) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        ft_putstr_fd("error: open\n", STDERR_FILENO);
        return NULL;
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        ft_putstr_fd("error: fstat\n", STDERR_FILENO);
        close(fd);
        return NULL;
    }
    *length = statbuf.st_size;

    void *addr = mmap(NULL, *length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        ft_putstr_fd("error: mmap\n", STDERR_FILENO);
        close(fd);
        return NULL;
    }

    close(fd);
    return addr;
}

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
        ft_printf("no section header string table\n");
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
        ft_printf("no section header string table\n");
        return NULL;
    }
    prop->string_table_index = ehdr->e_shstrndx;
}

void *hopla(const uint8_t *data, elf_prop_t *prop) {
    // Check if magic code is correct (ELF format)
    // Magic code: 0x7f, 'E', 'L', 'F'
    if (!(data[0] == 0x7f && data[1] == 'E' && data[2] == 'L' && data[3] == 'F')) {
        ft_printf("magic code wrong\n");
        return NULL;
    }

    if (data[4] == ELFCLASSNONE) {
        ft_printf("invalid class\n");
        return NULL;
    }

    // save arch and encoding information which will be essential for further parsing
    prop->arch = data[4];
    prop->encoding = data[5];

    void *res = (data[4] == ELFCLASS32) ? process_elf_header32(data, prop) : process_elf_header64(data, prop);
    if (!res) {
        ft_printf("error processing ELF header\n");
        return NULL;
    }
}

int run_nm(const char* path) {
    size_t len;
    void *addr = read_binary(path, &len);
    if (!addr)
        return 1;
    
    elf_prop_t prop;
    void *res = hopla(addr, &prop);
    if (!res) {
        munmap(addr, len);
        return 1;
    }

    if (prop.arch == ELFCLASS32) {
        ft_printf("32-bit ELF file\n");
    } else if (prop.arch == ELFCLASS64) {
        ft_printf("64-bit ELF file\n");
    } else {
        ft_printf("invalid class\n");
        return 1;
    }

    // display64(addr, &prop);

    munmap(addr, len);
    return 0;

}