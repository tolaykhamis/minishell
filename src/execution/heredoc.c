/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:52 by jomeltah          #+#    #+#             */
/*   Updated: 2026/05/21 19:23:58 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


static int	check_signal_hook(void)
{
	if (g_signal == 2)
	{
		rl_done = 1;
	}
	return (0);
}

static void	handle_heredoc_signal(t_shell *shell, int write_fd, char *line)
{
	if (g_signal == 2)
	{
		clean_before_exit(shell);
		free(line);
		close(write_fd);
		exit(130);
	}
}

static int	process_hd_l(t_redi *rd, t_shell *shell, int write_fd, char *line)
{
	char	*expanded;

	handle_heredoc_signal(shell, write_fd, line);
	if (!line)
		return (1);
	if (ft_strcmp(line, rd->f) == 0)
	{
		free(line);
		return (1);
	}
	if (!rd->has_quotes)
	{
		expanded = expand_parameter(line, shell);
		free(line);
		line = expanded;
	}
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
	return (0);
}

void	read_heredoc_input(t_redi *rd, t_shell *shell, int write_fd)
{
	char	*line;

	rl_event_hook = check_signal_hook;
	while (1)
	{
		line = readline("> ");
		if (process_hd_l(rd, shell, write_fd, line))
			break ;
	}
}

int	prepare_heredocs(t_cmdlist *cmds, t_shell *shell)
{
	t_cmdlist	*cmd;
	t_redi		*rd;

	cmd = cmds;
	while (cmd)
	{
		rd = cmd->redirs;
		while (rd)
		{
			if (rd->type == TOKEN_HERDOC)
			{
				rd->heredoc_fd = handle_heredoc(rd, shell);
				if (rd->heredoc_fd < 0)
				{
					close_opened_heredocs(cmds, rd);
					return (-1);
				}
			}
			rd = rd->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
