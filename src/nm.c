#include "nm.h"
#include "parse.h"
#include "display.h"

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

static void *read_binary(const char *path, size_t *length) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        ft_printf("ft_nm: Error opening file '%s'\n", path);
        return NULL;
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        
        close(fd);
        return NULL;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        ft_printf("ft_nm: Warning: '%s' is a directory\n", path);
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

static int retrieve_section_header_data_32(const uint8_t *data, elf_prop_t *prop, const char *path) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *) data;
    if (ehdr->e_shstrndx == SHN_UNDEF) {
        ft_printf("ft_nm: %s: no section header string table\n", path);
        return -1;
    }
    prop->string_table_index = ehdr->e_shstrndx;
    prop->section_header = (void *) data + ehdr->e_shoff;
    prop->section_entry_nb = ehdr->e_shnum;
    prop->section_entry_size = ehdr->e_shentsize;
    return 0;
}

static int retrieve_section_header_data_64(const uint8_t *data, elf_prop_t *prop, const char *path) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) data;
    if (ehdr->e_shstrndx == SHN_UNDEF) {
        ft_printf("ft_nm: %s: no section header string table\n", path);
        return -1;
    }
    prop->string_table_index = ehdr->e_shstrndx;
    prop->section_header = (void *) data + ehdr->e_shoff;
    prop->section_entry_nb = ehdr->e_shnum;
    prop->section_entry_size = ehdr->e_shentsize;
    return 0;
}

static int process_elf_header(const uint8_t *data, elf_prop_t *prop, const char *path) {
    // Check if magic code is correct (ELF format)
    // Magic code: 0x7f, 'E', 'L', 'F'
    if (!(data[0] == 0x7f && data[1] == 'E' && data[2] == 'L' && data[3] == 'F')) {
        ft_printf("ft_nm: %s: file format not recognized\n", path);
        return -1;
    }

    // Check if the architecture is supported
    if (data[4] == ELFCLASSNONE) {
        ft_printf("ft_nm: %s: invalid architecture\n", path);
        return -1;
    }

    // save arch and encoding information which will be essential for further parsing
    prop->arch = data[4];
    prop->encoding = data[5];

    // Retrieve information about the section header
    return (data[4] == ELFCLASS32) 
        ? retrieve_section_header_data_32(data, prop, path) 
        : retrieve_section_header_data_64(data, prop, path);
}


int run_nm(const char* path) {
    // Read the binary file
    size_t len;
    void *addr = read_binary(path, &len);
    if (!addr)
        return 1;
    
    // Process ELF header and retrieve information about the binary
    elf_prop_t prop;
    int res = process_elf_header(addr, &prop, path);
    if (res < 0) {
        munmap(addr, len);
        return 1;
    }

    // Display the name of the file
    if (g_file_nb > 1)
        ft_printf("%s:\n", path);

    // Extract symbol data in linked list
    t_list *symbol_data = (prop.arch == ELFCLASS32) ? extract_symbol_data_32(addr, &prop) : extract_symbol_data_64(addr, &prop);

    // Display the symbol data considering the options
    display_symbol_data(symbol_data);

    // Free & unmap
    //ft_lstclear(&symbol_data, free);
    munmap(addr, len);
    return 0;
}