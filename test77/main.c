/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:06 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:47:09 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = ft_calloc(i + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (i--)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	return (new_envp);
}

static void	process_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_cmdlist	*cmds;

	tokens = NULL;
	cmds = NULL;
	extract_token_list(line, &tokens, shell);
	if (!tokens)
		return ;
	if (!check_syntax(tokens))
	{
		shell->exit_status = 2;
		free_tokens(tokens);
		return ;
	}
	argumentssloop(tokens, &cmds);
	expander(&cmds, shell);
	shell->cmds = cmds;
	free_tokens(tokens);
	execute_command(shell);
	shell->cmds = NULL;
	freeargs(cmds); 
}

static int	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("\033[0;32mminishell> ");
		if (!line)
		{
			write(1,"exit\n",5);
			break ;
		}
		if (g_signal == SIGINT)
			shell->exit_status = 130;
		if (g_signal == SIGQUIT)
			shell->exit_status = 131;
		g_signal = 0; 
		if (*line)
		{
			add_history(line);
			process_line(line, shell);
		}
		free(line);
	}
	return (shell->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		exit_s;

	(void)argc;
	(void)argv;
	shell.envp = copy_envp(envp);
	shell.export = copy_envp(envp);
	shell.exit_status = 0;
	shell.cmds = NULL;
	shell.pids = NULL;
	print_welcome();
	setup_signals_interactive();
	exit_s = shell_loop(&shell);
	clean_before_exit(&shell);
	return (exit_s);
}
