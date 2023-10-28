#include "libft.h"

static void *ft_memset(void *b, int c, size_t len) {
	char *p = b;
	while (len-- > 0)
		*p++ = c;
	return (b);
}

static void ft_bzero(void *s, size_t n) { ft_memset(s, 0, n); }

void *ft_calloc(size_t count, size_t size) {
	void *res = malloc(count * size);
	if (!res)
		return (NULL);
	ft_bzero(res, count * size);
	return (res);
}