#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct s_list {
    void *content;
    struct s_list *next;
}	t_list;

void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
int ft_printf(const char *s, ...);
void *ft_calloc(size_t count, size_t size);
int ft_strcmp(const char *s1, const char *s2);
t_list *ft_lstnew(void *content);
void ft_lstadd_back(t_list **alst, t_list *new_list);
int ft_lstsize(t_list *lst);
void ft_lstsort(t_list **alst, int OPTs, int (*cmp)(void *, void *, int));
void ft_lstclear(t_list **lst, void (*del)(void *));