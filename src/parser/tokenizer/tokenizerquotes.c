/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizerquotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:12:49 by tkhamis           #+#    #+#             */
/*   Updated: 2026/05/21 19:23:58 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


static int	deli(char c, int s, int q)
{
	if (!s && !q && (ft_space(c) || is_operator(c)))
		return (1);
	return (0);
}

int	word_length(char *ptr)
{
	int	len;
	int	s;
	int	d;

	len = 0;
	s = 0;
	d = 0;
	while (*ptr)
	{
		if (deli(*ptr, s, d))
			break ;
		if (*ptr == '\'' && !d)
			s = !s;
		else if (*ptr == '"' && !s)
			d = !d;
		len++;
		ptr++;
	}
	if (s || d)
		return (-1);
	return (len);
}

char	*quotes2(char **ptr, char *word)
{
	int	i;
	int	s;
	int	q;

	i = 0;
	s = 0;
	q = 0;
	while (**ptr)
	{
		if (deli(**ptr, s, q))
			break ;
		if (**ptr == '\'' && !q)
			s = !s;
		else if (**ptr == '"' && !s)
			q = !q;
		word[i++] = **ptr;
		(*ptr)++;
	}
	word[i] = '\0';
	return (word);
}

char	*quote(char **ptr, t_shell *shell)
{
	char	*word;
	int		len;

	len = word_length(*ptr);
	if (len < 0)
	{
		printf("minishell: syntax error unclosed quotes\n");
		shell->exit_status = 2;
		while (**ptr)
			(*ptr)++;
		return (NULL);
	}
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	word = quotes2(ptr, word);
	return (word);
}
