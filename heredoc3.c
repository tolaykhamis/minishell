/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:52:03 by tkhamis           #+#    #+#             */
/*   Updated: 2026/04/27 17:52:04 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child(t_redi *rd, t_shell *shell, int *pipe_fd)
{
	setup_signals_heredoc();
	close(pipe_fd[0]);
	read_heredoc_input(rd, shell, pipe_fd[1]);
	clean_before_exit(shell);
	close(pipe_fd[1]);
	exit(0);
}

static int	handle_heredoc_parent(t_shell *shell, int *pipe_fd, pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_signal = 0;
		shell->exit_status = 130;
		close(pipe_fd[0]);
		write(STDOUT_FILENO, "\n", 1);
		return (-1);
	}
	setup_signals_interactive();
	return (pipe_fd[0]);
}

int	handle_heredoc(t_redi *rd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
		heredoc_child(rd, shell, pipe_fd);
	return (handle_heredoc_parent(shell, pipe_fd, pid));
}
