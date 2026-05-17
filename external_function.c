/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_function.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:35 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/26 19:48:07 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*command_path(char *cmd, char **env, int i, t_shell *shell)
{
	char	*path_env;
	char	**all_paths;
	char	*tmp;
	char	*full_path;

	(void)shell;
	path_env = get_env_name(env, "PATH");
	if (!path_env)
		return (NULL);
	all_paths = ft_split(path_env, ':');
	while (all_paths && all_paths[i])
	{
		tmp = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(full_path, X_OK))
		{
			free_free(all_paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_free(all_paths);
	return (NULL);
}

static void	go_child(char *path, t_cmdlist *cmd, t_shell *shell)
{
	setup_signals_child();
	close(cmd->saved_stdout);
	close(cmd->saved_stdin);
	execve(path, cmd->av, shell->envp);
	perror(cmd->av[0]);
	if (path != cmd->av[0])
		free(path);
	clean_before_exit(shell);
	exit(126);
}

static void	wait_ya_parent(pid_t pid, int *status, t_shell *shell)
{
	setup_signals_child();
	if (g_signal == SIGINT)
	{
		shell->exit_status = 130;
	}
	if (g_signal == SIGQUIT)
	{
		shell->exit_status = 131;
	}
	waitpid(pid, status, 0);
	if ((*status >> 8) == 130)
		shell->exit_status = 130;
	setup_signals_interactive();
}

static void	family_time(pid_t pid, t_shell *shell, t_cmdlist *cmd, char *path)
{
	int	status;

	if (pid == 0)
		go_child(path, cmd, shell);
	else if (pid > 0)
		wait_ya_parent(pid, &status, shell);
	else
	{
		perror("fork");
		shell->exit_status = 1;
	}
}

void	execute_external(t_shell *shell, t_cmdlist *cmd)
{
	pid_t	pid;
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
		shell->exit_status = 127;
		return ;
	}
	pid = fork();
	family_time(pid, shell, cmd, path);
	if (path != cmd->av[0])
		free(path);
}
