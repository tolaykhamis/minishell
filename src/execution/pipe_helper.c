/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:22 by jomeltah          #+#    #+#             */
/*   Updated: 2026/05/21 19:23:58 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


int	cmds_len(t_cmdlist *cmds)
{
	int			i;
	t_cmdlist	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

pid_t	*init_pids(int count)
{
	pid_t	*pids;

	pids = ft_calloc(count, sizeof(pid_t));
	return (pids);
}

void	wait_all(t_shell *shell, pid_t *pids, int count)
{
	int		i;
	int		status;
	pid_t	pp;

	pp = 0;
	i = 0;
	while (i < count)
	{
		pp = wait(&status);
		if (pp == -1)
			break ;
		if (pp == pids[count - 1])
		{
			if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
			else if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
		}
		i++;
	}
}
