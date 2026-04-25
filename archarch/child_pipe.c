#include "minishell.h"

static void	setup_fds(t_cmdlist *cmd, int prev_fd, int *pipe_fd)
{
	if (prev_fd != STDIN_FILENO)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next && pipe_fd)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

static void	handle_redirections(t_shell *shell, t_cmdlist *cmd)
{
	if (cmd->redirs)
	{
		if (apply_redirections(cmd, shell) < 0)
		{
			clean_before_exit(shell);
			exit(1);
		}
	}
}

static void	handle_builtin(t_shell *shell, t_cmdlist *cmd)
{
	int	status;

	init_isbuiltin(cmd);
	if (!cmd->is_builtin)
		return ;
	if (ft_strcmp(cmd->av[0], "exit") == 0)
	{
		if (cmd->av[1])
			status = ft_atoi(cmd->av[1]);
		else
			status = shell->exit_status;
	}
	else
		status = execute_builtin(shell, cmd);
	clean_before_exit(shell);
	exit(status);
}

static void	resolve_and_exec(t_shell *shell, t_cmdlist *cmd)
{
	char	*path;

	if (ft_strchr(cmd->av[0], '/'))
		path = cmd->av[0];
	else
		path = command_path(cmd->av[0], shell->envp, 0, shell);
	if (!path)
	{
		put_str_fd("minishell: ", 2);
		put_str_fd(cmd->av[0], 2);
		put_str_fd(": command not found\n", 2);
		clean_before_exit(shell);
		exit(127);
	}
	execve(path, cmd->av, shell->envp);
	if (path != cmd->av[0])
		free(path);
	perror(cmd->av[0]);
	clean_before_exit(shell);
	exit(126);
}

void	child_process(t_shell *shell, t_cmdlist *cmd,
		int prev_fd, int *pipe_fd)
{
	if (!cmd->av || !cmd->av[0])
	{
		clean_before_exit(shell);
		exit(0);
	}
	setup_fds(cmd, prev_fd, pipe_fd);
	handle_redirections(shell, cmd);
	handle_builtin(shell, cmd);
	resolve_and_exec(shell, cmd);
	free(shell->pids);
}
