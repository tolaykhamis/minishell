NAME		= minishell

CC		= cc
LDFLAGS = -lreadline -L/usr/local/opt/readline/lib
CFLAGS  = -Wall -Wextra -Werror -I/usr/local/opt/readline/include

SRCS		= main.c \
		  tokenizer.c \
		  tokenizerfiles.c \
		  tokenizerops.c \
		  tokenizerquotes.c \
		  parsing.c \
		  parsingredirect.c \
		  expansion.c \
		  errorhandler.c \
		  helpersforerror.c \
		  builtin_functions.c \
		  external_function.c \
		  init_t_command.c \
		  path.c \
		  frees.c \
		  red.c \
		  signals.c \
		  utils.c \
		  helpme.c \
		  join.c \
		  pipe.c \
		  execution.c \
		  readtheline.c

OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o:		%.c minishell.h
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS)

fclean:		clean
		rm -f $(NAME)

re:		fclean all

.PHONY:		all clean fclean re