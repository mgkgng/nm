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

int main(int argc, char **argv) {
    (void) argc;
    
    size_t len;
    void *addr = read_binary(argv[1], &len);
    if (!addr) {
        return 1;
    }
    
    elf_prop_t prop;
    parse(addr, &prop);

    munmap(addr, len);
    return 0;
}