CC = gcc
SANITIZE = -g -fsanitize=address
CFLAGS = -Wall -Wextra -Werror

SRCDIR = src
INCDIR = include
OBJDIR = obj

LIB = libft.a

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(LIB)

re: fclean all

.PHONY: all clean fclean re
