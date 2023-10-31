#include "nm.h"
#include "parse.h"
#include "libft.h"

int main(int argc, char **argv) {
    if (argc < 2)
        return run_nm("a.out");

    t_list *files = parse(argc, argv);
    if (!files)
        return (g_file_nb) ? run_nm("a.out") : 1;

    int res;
    while (files) {
        res = run_nm(files->content);
        files = files->next;
        if (files)
            ft_printf("\n");
    }
    ft_lstclear(&files, free);
    return res;
}