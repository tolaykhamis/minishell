/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fun1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:02:48 by jomeltah          #+#    #+#             */
/*   Updated: 2026/05/21 19:23:31 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_pwd(t_shell *shell)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*tmp;

	oldpwd = get_env_name(shell->envp, "PWD");
	if (getcwd(cwd, sizeof(cwd)))
	{
		if (oldpwd)
		{
			tmp = ft_strjoin("OLDPWD=", oldpwd);
			shell->envp = add_to_envp(shell->envp, tmp);
			free(tmp);
		}
		tmp = ft_strjoin("PWD=", cwd);
		shell->envp = add_to_envp(shell->envp, tmp);
		free(tmp);
	}
}

static int	count_argss(t_cmdlist *cmd)
{
	int		i;

	i = 0;
	while (cmd && cmd->av[i])
		i++;
	if (i != 2)
	{
		put_str_fd("cd: too many arguments\n", 2);
	}
	return (i - 1);
}

int	builtin_cd(t_shell *shell, t_cmdlist *cmd)
{
	char	*path;
	int		args_num;

	args_num = count_argss(cmd);
	if (args_num != 1)
		return (1);
	if (!cmd->av[1])
		path = get_env_name(shell->envp, "HOME");
	else
		path = cmd->av[1];
	if (!path)
	{
		put_str_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(path))
	{
		put_str_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_pwd(shell);
	return (0);
}

int	builtin_pwd(void)
{
	char	pwd_output[1024];

	if (getcwd(pwd_output, sizeof(pwd_output)))
	{
		put_str_fd(pwd_output, 1);
		put_str_fd("\n", 1);
		return (0);
	}
	else
		perror("pwd");
	return (1);
}
