/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:46 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:04:47 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_fd(const char *file, int flags, mode_t mode, int target_fd)
{
	int	fd;

	fd = open(file, flags, mode);
	if (fd < 0)
	{
		put_str_fd("minishell: ", 2);
		put_str_fd((char *)file, 2);
		put_str_fd(": ", 2);
		perror(NULL);
		return (-1);
	}
	dup2(fd, target_fd);
	close(fd);
	return (0);
}

static int	handle_single_redir(t_redi *rd, t_shell *shell)
{
	if (rd->type == TOKEN_REDIRECT_IN)
	{
		if (redirect_fd(rd->f, O_RDONLY, 0, STDIN_FILENO) < 0)
			return (-1);
	}
	else if (rd->type == TOKEN_REDIRECT_OUT)
	{
		if (redirect_fd(rd->f, O_WRONLY | O_CREAT | O_TRUNC, 0644,
				STDOUT_FILENO) < 0)
			return (-1);
	}
	else if (rd->type == TOKEN_APPEND)
	{
		if (redirect_fd(rd->f, O_WRONLY | O_CREAT | O_APPEND, 0644,
				STDOUT_FILENO) < 0)
			return (-1);
	}
	else if (rd->type == TOKEN_HERDOC)
	{
		if (apply_heredoc(rd, shell) < 0)
			return (-1);
	}
	return (0);
}

static int	handle_heredoc_redir(t_redi *rd, t_shell *shell)
{
	t_redi	*tmp;
	int		last;

	tmp = rd->next;
	last = 1;
	while (tmp)
	{
		if (tmp->type == TOKEN_HERDOC)
		{
			last = 0;
			break ;
		}
		tmp = tmp->next;
	}
	if (last)
		return (apply_heredoc(rd, shell));
	close(rd->heredoc_fd);
	return (0);
}

int	apply_redirections(t_cmdlist *cmd, t_shell *shell)
{
	t_redi	*rd;

	rd = cmd->redirs;
	while (rd)
	{
		if (rd->type == TOKEN_HERDOC)
		{
			if (handle_heredoc_redir(rd, shell) < 0)
				return (-1);
		}
		else
		{
			if (handle_single_redir(rd, shell) < 0)
				return (-1);
		}
		rd = rd->next;
	}
	return (0);
}
