/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readtheline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:48 by tkhamis           #+#    #+#             */
/*   Updated: 2026/05/21 19:24:20 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	fill(char **res, const char *s, char c)
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

void	split_line(char *line)
{
	char	**tokens;
	int		i;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		printf("Token: %s\n", tokens[i]);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

const char	*token_name(enum e_token type)
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
