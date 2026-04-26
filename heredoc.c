/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:52 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/26 19:32:46 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_all_heredocs(t_cmdlist *all_cmds) // add this tolay
{
    t_cmdlist   *tmp;
    t_redi      *rd;

    tmp = all_cmds;
    while (tmp)
    {
        rd = tmp->redirs;
        while (rd)
        {
            if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
            {
                close(rd->heredoc_fd);
                rd->heredoc_fd = -1;
            }
            rd = rd->next;
        }
        tmp = tmp->next;
    }
}
static int	check_signal_hook(void)
{
    if (g_signal == 2)
    {
        rl_done = 1;
    }
    return (0);
}
static void	read_heredoc_input(t_redi *rd, t_shell *shell, int write_fd)
{
	char	*line;
	char	*expanded;

	rl_event_hook = check_signal_hook;
	while (1)
	{
		line = readline("> ");
		if(g_signal == 2) // add this condition
		{
			// g_signal = 0; remove this
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
	pid = fork();
	if (pid == 0)
	{
		setup_signals_heredoc();
		close(pipe_fd[0]);
		read_heredoc_input(rd, shell, pipe_fd[1]);
		clean_before_exit(shell);
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_signal = 0;
		// printf("HEEEEEEEERRR");
		shell->exit_status = 130;
    	close(pipe_fd[0]);
		// kill(pid,SIGINT); remove this line
		write(STDOUT_FILENO, "\n", 1); // 1 -> stdout
    	return (-1);
	}
	setup_signals_interactive(); // add this before wait not after
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
// add this function
void close_unused_heredocs(t_cmdlist *cmd)
{
    t_redi *rd;

    rd = cmd->redirs;
    while (rd)
    {
        if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
        {
            close(rd->heredoc_fd);
            rd->heredoc_fd = -1;
        }
        rd = rd->next;
    }
}

// void close_all_heredocs(t_cmdlist *all_cmds) tolay
// {
//     t_cmdlist   *tmp;
//     t_redi      *rd;

//     tmp = all_cmds;
//     while (tmp)
//     {
//         rd = tmp->redirs;
//         while (rd)
//         {
//             if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
//             {
//                 close(rd->heredoc_fd);
//                 rd->heredoc_fd = -1;
//             }
//             rd = rd->next;
//         }
//         tmp = tmp->next;
//     }
// }
// add this function
// static void close_opened_heredocs(t_cmdlist *cmds, t_redi *stop)
// {
//     t_cmdlist   *cmd;
//     t_redi      *rd;

//     cmd = cmds;
//     while (cmd)
//     {
//         rd = cmd->redirs;
//         while (rd && rd != stop)
//         {
//             if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
//             {
//                 close(rd->heredoc_fd);
//                 rd->heredoc_fd = -1;
//             }
//             rd = rd->next;
//         }
//         cmd = cmd->next;
//     }
// }
// replace it with this bellow
void close_opened_heredocs(t_cmdlist *cmds, t_redi *stop)
{
    t_cmdlist   *cmd;
    t_redi      *rd;
    int         found;

    found = 0;
    cmd = cmds;
    while (cmd)
    {
        rd = cmd->redirs;
        while (rd)
        {
            if (rd == stop)
            {
                found = 1;
                break ;
            }
            if (rd->type == TOKEN_HERDOC && rd->heredoc_fd >= 0)
            {
                close(rd->heredoc_fd);
                rd->heredoc_fd = -1;
            }
            rd = rd->next;
        }
        if (found)
            break ;
        cmd = cmd->next;
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
					// clean_before_exit(shell);
					return (-1);
				}
			}
			rd = rd->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
