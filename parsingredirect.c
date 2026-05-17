/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingredirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:21:50 by tkhamis           #+#    #+#             */
/*   Updated: 2026/04/25 16:10:49 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(enum e_token type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HERDOC);
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKENPIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (is_redir(tokens->type))
		{
			if (tokens->next)
				tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (count);
}

void	addnode_redi(t_cmdlist *cmd, t_token *token)
{
	t_redi	*new_node;
	t_redi	*tmp;

	new_node = ft_calloc(1, sizeof(t_redi));
	if (!new_node || !token->next)
		return ;
	new_node->type = token->type;
	if (new_node->type == TOKEN_HERDOC)
	{
		if (ft_strchr(token->next->value, '\'') || ft_strchr(token->next->value,
				'\"'))
			new_node->has_quotes = 1;
	}
	new_node->f = ft_strdup(token->next->value);
	if (!cmd->redirs)
		cmd->redirs = new_node;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}
