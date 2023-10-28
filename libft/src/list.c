#include "libft.h"

t_list *ft_lstnew(void *content) {
	t_list	*res = ft_calloc(1, sizeof(t_list));;
	if (!res)
		return (NULL);
	res->content = content;
	return (res);
}

void ft_lstadd_back(t_list **alst, t_list *new_list) {
	if (!*alst) {
		*alst = new_list;
		return ;
	}

	t_list *begin = *alst;
	while (begin->next)
		begin = begin->next;
	begin->next = new_list;
}

void ft_lstsort(t_list **alst, int flags, int (*cmp)(void *, void *, int)) {
	t_list *begin = *alst;
	t_list *tmp = NULL;
	while (begin) {
		tmp = begin->next;
		while (tmp) {
			if (cmp(begin->content, tmp->content, flags) > 0) {
				void *tmp_content = begin->content;
				begin->content = tmp->content;
				tmp->content = tmp_content;
			}
			tmp = tmp->next;
		}
		begin = begin->next;
	}
}

int ft_lstsize(t_list *lst) {
	int res = 0;
	while (lst) {
		res++;
		lst = lst->next;
	}
	return (res);
}
