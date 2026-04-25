/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:52 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:03:53 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// add this function bellow

static void    close_all_heredoc_fds(t_cmdlist *cmds) 
{
    t_cmdlist    *cmd;
    t_redi        *rd;
    cmd = cmds;
    while (cmd)
    {
        rd = cmd->redirs;
        while (rd)
        {
            if (rd->heredoc_fd >= 0)
            {
                close(rd->heredoc_fd);
                rd->heredoc_fd = -1;
            }
            rd = rd->next;
        }
        cmd = cmd->next;
    }
}

static void	read_heredoc_input(t_redi *rd,t_shell *shell, int write_fd)
{
	char	*line;
	char	*expanded;

	// rl_catch_signals = 0; detete this 
	while (1)
	{
		line = readline("> ");
		// printf("YES = %d\n ",g_signal); // add this
		if(g_signal == 2) // add this condition
		{
			g_signal = 0;
			clean_before_exit(shell);
			free(line);
			close(write_fd);
			exit(130);
		}	
		if (!line)
			break ;
		if (ft_strcmp(line, rd->f) == 0)
		{
			free(line);
			break ;
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
	}
}

static int	handle_heredoc(t_redi *rd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) < 0)
		return (-1);
	// signal(SIGINT, SIG_IGN);// delete this 	
	pid = fork();
	if (pid == 0)
	{
		setup_signals_heredoc();
		close(pipe_fd[0]);
		read_heredoc_input(rd,shell, pipe_fd[1]);
		clean_before_exit(shell);
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_signals_interactive(); // add this
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)	// replace from if((status >> 8) == 0)
	{
		shell->exit_status = 130;
		close(pipe_fd[0]);
		write(1, "\n", 1);
		return (-1);
	}
	return (pipe_fd[0]);
}

int	apply_heredoc(t_redi *rd, t_shell *shell)
{
	(void)shell;
	if (dup2(rd->heredoc_fd, STDIN_FILENO) < 0)
		return (-1);
	close(rd->heredoc_fd);
	return (0);
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
					close_all_heredoc_fds(cmds); // add this line
					return (-1);
				}
			}
			rd = rd->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
