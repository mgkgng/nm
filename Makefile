CC = gcc
# CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude -Ilibft/include

SRCDIR = src
BUILDDIR = obj
LIBFTDIR = libft

TARGET = ft_nm

LIBFT = $(LIBFTDIR)/libft.a

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: $(LIBFT) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBFT) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

clean:
	rm -rf $(BUILDDIR)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(TARGET)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re