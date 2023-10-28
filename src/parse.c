#include "parse.h"
#include "libft.h"

uint8_t g_opts = 0;
uint8_t g_file_nb = 0;

static uint8_t get_opt(char *s) {
    uint8_t res = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] == 'A')
            res |= OPT_A;
        else if (s[i] == 'G')
            res |= OPT_G;
        else if (s[i] == 'U')
            res |= OPT_U;
        else if (s[i] == 'R')
            res |= OPT_R;
        else if (s[i] == 'P')
            res |= OPT_P;
        else {
            ft_putstr_fd("nm: invalid option -- '", STDERR_FILENO);
            ft_putchar_fd(s[i], STDERR_FILENO);
            ft_putstr_fd("'\n", STDERR_FILENO);
            return -1;
        }
    }
    return res;
}

t_list *parse(int ac, char **av) {
    t_list *res = NULL;
    for (int i= 1; i < ac; i++) {
        if (av[i][0] == '-' && av[i][1] != '\0') {
            uint8_t opt = get_opt(av[i] + 1);
            if (opt == -1) {
                g_file_nb = 0;
                return NULL;
            }
            g_opts |= opt;
        } else {
            ft_lstadd_back(&res, ft_lstnew(av[i]));
            g_file_nb++;
        }
    }
    return res;
}