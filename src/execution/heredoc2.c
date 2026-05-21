/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:51:57 by tkhamis           #+#    #+#             */
/*   Updated: 2026/05/21 19:23:58 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void	close_all_heredocs(t_cmdlist *all_cmds)
{
	t_cmdlist	*tmp;
	t_redi		*rd;

	tmp = all_cmds;
	while (tmp)
	{
		rd = tmp->redirs;
		while (rd)
		{
			if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
			{
				close(rd->heredoc_fd);
				rd->heredoc_fd = -1;
			}
			rd = rd->next;
		}
		tmp = tmp->next;
	}
}

void	close_unused_heredocs(t_cmdlist *cmd)
{
	t_redi	*rd;

	rd = cmd->redirs;
	while (rd)
	{
		if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
		{
			close(rd->heredoc_fd);
			rd->heredoc_fd = -1;
		}
		rd = rd->next;
	}
}

static int	close_heredocs_in_cmd(t_redi *rd, t_redi *stop)
{
	while (rd)
	{
		if (rd == stop)
			return (1);
		if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
		{
			close(rd->heredoc_fd);
			rd->heredoc_fd = -1;
		}
		rd = rd->next;
	}
	return (0);
}

void	close_opened_heredocs(t_cmdlist *cmds, t_redi *stop)
{
	t_cmdlist	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (close_heredocs_in_cmd(cmd->redirs, stop))
			break ;
		cmd = cmd->next;
	}
}

int	apply_heredoc(t_redi *rd, t_shell *shell)
{
	(void)shell;
	if (dup2(rd->heredoc_fd, STDIN_FILENO) < 0)
		return (-1);
	close(rd->heredoc_fd);
	return (0);
}
