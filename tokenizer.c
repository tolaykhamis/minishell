/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:55 by tkhamis           #+#    #+#             */
/*   Updated: 2026/04/10 13:19:03 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

t_token	*new_token(char *value, enum token type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **list, t_token *new)
{
	t_token	*tmp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

char	*substr_dup(char *start, int len)
{
	char	*str;
	int		i;

	str = malloc(len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = start[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	handle_word(char **ptr, t_token **list, t_shell *shell)
{
	char	*word;

	word = quote(ptr, shell);
	if (!word)
		return ;
	add_token(list, new_token(word, TOKEN_WORD));
}

void	extract_token_list(char *line, t_token **token_list, t_shell *shell)
{
	char	*ptr;

	ptr = line;
	while (*ptr)
	{
		while (*ptr && ft_space(*ptr))
			ptr++;
		if (!*ptr)
			break ;
		if (is_operator(*ptr))
			handle_operator(&ptr, token_list);
		else
			handle_word(&ptr, token_list, shell);
	}
}
