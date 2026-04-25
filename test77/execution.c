/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:17 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:03:18 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_shell *shell)
{
	t_cmdlist	*cmd;

	cmd = shell->cmds;
	if (cmd == NULL)
		return ;
	if (cmd->next == NULL)
	{
		if (prepare_heredocs(shell->cmds, shell) < 0)
			return ;
		single_execution(shell, cmd);
	}
	else
	{
		if (prepare_heredocs(shell->cmds, shell) < 0)
			return ;
		pipeline_execution(shell);
	}
}

static int	prepare_execution(t_shell *shell, t_cmdlist *cmd, int saved_stdin,
		int saved_stdout)
{
	if (cmd->redirs)
	{
		if (apply_redirections(cmd, shell) < 0)
		{
			shell->exit_status = 1;
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			return (0);
		}
	}
	if (!cmd->av || !cmd->av[0])
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (0);
	}
	return (1);
}

static void	execute_cmd(t_shell *shell, t_cmdlist *cmd)
{
	if (cmd->is_builtin)
		shell->exit_status = execute_builtin(shell, cmd);
	else
		execute_external(shell, cmd);
}

void	single_execution(t_shell *shell, t_cmdlist *cmd)
{
	cmd->saved_stdin = dup(STDIN_FILENO);
	cmd->saved_stdout = dup(STDOUT_FILENO);
	init_isbuiltin(cmd);
	if (!prepare_execution(shell, cmd, cmd->saved_stdin, cmd->saved_stdout))
		return ;
	execute_cmd(shell, cmd);
	dup2(cmd->saved_stdin, STDIN_FILENO);
	dup2(cmd->saved_stdout, STDOUT_FILENO);
	close(cmd->saved_stdin);
	close(cmd->saved_stdout);
}
