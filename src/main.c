#include <stdio.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "parse.h"
#include "libft.h"
#include "nm.h"

uint8_t g_opts = 0;

int main(int argc, char **argv) {
    if (argc < 2)
        return run_nm("a.out");

    t_list *files = parse(argc, argv);

    return 0;       
}