NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I.
AR = ar rcs

SRCS = ft_strlen.c \
	ft_strdup.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_strncmp.c \
	ft_strchr.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_split.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_calloc.c \
	ft_memcmp.c \
	ft_itoa.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
