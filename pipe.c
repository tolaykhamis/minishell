/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:31 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/26 19:31:23 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipeline_cmd(t_shell *shell, t_cmdlist *cmd, int *pipe_fd,
		pid_t *pid)
{
	*pid = fork();
	if (*pid == 0)
	{
		setup_signals_child();
		child_process(shell,cmd, shell->prev_fd, pipe_fd);
		exit(shell->exit_status);
	}

	if (shell->prev_fd != STDIN_FILENO)
		close(shell->prev_fd);

	if (pipe_fd)
		close(pipe_fd[1]);
}

static void	pipeline_loop(t_shell *shell, int count)
{
	t_cmdlist	*cmd;
	int			pipe_fd[2];
	int			i;

	cmd = shell->cmds;
	shell->prev_fd = STDIN_FILENO;
	i = 0;
	while (cmd)
	{
		if (cmd->next)
			pipe(pipe_fd);
		if (cmd->next)
			execute_pipeline_cmd(shell,cmd, pipe_fd, &shell->pids[i]);
		else
			execute_pipeline_cmd(shell,cmd, NULL, &shell->pids[i]);
		// close_unused_heredocs(cmd); // add this 	// 25
		setup_signals_child();
        if (g_signal == SIGINT)
		    shell->exit_status = 130;
	    if (g_signal == SIGQUIT)
		    shell->exit_status = 131;
		if (cmd->next)
			shell->prev_fd = pipe_fd[0];
		cmd = cmd->next;
		i++;
	}
	wait_all(shell, shell->pids, count); // remove last param pid_fd
	close_all_heredocs(shell->cmds); 
	setup_signals_interactive();
}

void	pipeline_execution(t_shell *shell)
{
	int	count;

	count = cmds_len(shell->cmds);
	shell->pids = init_pids(count);
	if (!shell->pids)
		return ;

	pipeline_loop(shell,count);
	clear_pids(shell); //delete this // return it
}
