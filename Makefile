
NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror -g -I/usr/local/opt/readline/include
LDFLAGS := -lreadline -L/usr/local/opt/readline/lib

SRCS := $(shell find src -name '*.c' -print)
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
