/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:41 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/07 22:52:41 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_cmd_data(t_token *cur, t_cmdlist *cmd)
{
	int	i;

	i = 0;
	while (cur && cur->type != TOKENPIPE)
	{
		if (cur->type == TOKEN_WORD)
			cmd->av[i++] = ft_strdup(cur->value);
		else if (is_redir(cur->type))
		{
			if (cur->next && cur->next->type == TOKEN_WORD)
			{
				addnode_redi(cmd, cur);
				cur = cur->next;
			}
		}
		cur = cur->next;
	}
	cmd->av[i] = NULL;
}

static void	add_cmd_to_list(t_cmdlist **cmds, t_cmdlist *new_cmd)
{
	t_cmdlist	*tmp;

	if (!*cmds)
		*cmds = new_cmd;
	else
	{
		tmp = *cmds;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_cmd;
	}
}

void	handle_cmds(t_token *tokens, t_cmdlist **cmds)
{
	t_cmdlist	*cmd;

	cmd = malloc(sizeof(t_cmdlist));
	if (!cmd)
		return ;
	cmd->av = malloc(sizeof(char *) * (count_args(tokens) + 1));
	if (!cmd->av)
	{
		free(cmd);
		return ;
	}
	cmd->redirs = NULL;
	cmd->next = NULL;
	fill_cmd_data(tokens, cmd);
	add_cmd_to_list(cmds, cmd);
}

void	argumentssloop(t_token *tokens, t_cmdlist **cmds)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		handle_cmds(cur, cmds);
		while (cur && cur->type != TOKENPIPE)
			cur = cur->next;
		if (cur && cur->type == TOKENPIPE)
			cur = cur->next;
	}
}
