NAME		= minishell

CC		= cc
LDFLAGS = -lreadline -L/usr/local/opt/readline/lib
CFLAGS  = -Wall -Wextra -Werror -I/usr/local/opt/readline/include -g

SRCS		= main.c \
		  tokenizer.c \
		  tokenizerfiles.c \
		  tokenizerops.c \
		  tokenizerquotes.c \
		  parsing.c \
		  parsingredirect.c \
		  expansion.c \
		  expansion2.c \
		  errorhandler.c \
		  helpersforerror.c \
		  builtin_functions.c \
		  external_function.c \
		  init_t_command.c \
		  frees.c \
		  red.c \
		  signals.c \
		  utils.c \
		  helpme.c \
		  helpme2.c \
		  helpme3.c \
		  join.c \
		  pipe.c \
		  editing_env_path.c \
		  builtin_fun1.c \
		  builtin_fun2.c \
		  execution.c \
		  readtheline.c \
		  heredoc.c \
		  export.c \
		  test.c \
		  pipe_helper.c \
		  readtheline2.c

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
