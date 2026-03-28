/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readtheline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:48 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/07 22:52:48 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void free_split(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}
static int	count_words(const char *s, char c)
{
	int	words;
	int	count;

	words = 0;
	count = 0;
	while (*s)
	{
		if (*s != c && words == 0)
		{
			words = 1;
			count++;
		}
		if (*s == c)
			words = 0;
		s++;
	}
	return (count);
}

static char	*word_dup(const char *s, size_t len)
{
	char	*word;
	size_t	i;

	i = 0;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	free_i(char **res, int i)
{
	while (i >= 0)
	{
		free(res[i]);
		i--;
	}
	free(res);
}

static int	fill(char **res, const char *s, char c)
{
	const char	*start;
	size_t		len;
	int			i;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		start = s;
		while (*s && *s != c)
			s++;
		len = (size_t)(s - start);
		res[i] = word_dup(start, (int)len);
		if (!res[i])
		{
			free_i(res, i - 1);
			return (0);
		}
		i++;
	}
	res[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		words;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	res = malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	if (!fill(res, s, c))
		return (NULL);
	return (res);
}
void split_line(char *line)
{
    char **tokens;
    int i;

    tokens = ft_split(line, ' ');
    if (!tokens)
        return;

    i = 0;
    while (tokens[i])
    {
        printf("Token: %s\n", tokens[i]);
        free(tokens[i]);
        i++;
    }
    free(tokens);
}
static const char *token_name(enum token type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKENPIPE)
		return ("PIPE");
	if (type == TOKEN_REDIRECT_IN)
		return ("REDIRECT_IN");
	if (type == TOKEN_REDIRECT_OUT)
		return ("REDIRECT_OUT");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	if (type == TOKEN_HERDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}
void debug_tokens(t_token *tokens)
{
	int i = 0;

	printf("\n===== TOKENS =====\n");

	while (tokens)
	{
		printf("[%d] type=%s  value=\"%s\"\n",
			i,
			token_name(tokens->type),
			tokens->value);
		tokens = tokens->next;
		i++;
	}

	printf("==================\n\n");
}
void debug_commands(t_cmdlist *cmds)
{
	int		i;
	int		cmd_count = 0;
	t_redi	*r;

	printf("\n=== COMMAND TABLE DEBUG ===\n");
	while (cmds)
	{
		printf("Command [%d]:\n", cmd_count);
		if (cmds->av)
		{
			i = 0;
			while (cmds->av[i])
			{
				printf("  av[%d] = \"%s\"\n", i, cmds->av[i]);
				i++;
			}
		}
		r = cmds->redirs;
		while (r)
		{
			printf("  redir: type = %s, file = \"%s\"\n", 
				convert(r->type), r->f);
			r = r->next;
		}

		cmds = cmds->next;
		cmd_count++;
		printf("---------------------------\n");
	}
	printf("=============================\n\n");
}
void debug_pipeline(t_cmdlist *cmds)
{
	printf("\nPIPELINE VIEW:\n");

	while (cmds)
	{
		if (cmds->av)
			printf("[%s]", cmds->av[0]);

		if (cmds->next)
			printf("  |  ");

		cmds = cmds->next;
	}

	printf("\n\n");
}

