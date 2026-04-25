/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:22 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:04:23 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
			if ((status >> 8) == 130)
			{
				shell->exit_status = 130;
				return;
			}
		}
		i++;
	}
}
