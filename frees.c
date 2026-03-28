/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:20:36 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/19 16:20:36 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void free_cmds(t_cmdlist *cmds)
{
    t_cmdlist *tmp;
    int        i;

    while (cmds)
    {
        tmp = cmds->next;
        if (cmds->av)
        {
            i = 0;
            while (cmds->av[i])
            {
                free(cmds->av[i]);
                i++;
            }
            free(cmds->av);
        }
        if (cmds->redirs)
            free_redirs(cmds->redirs);
        free(cmds);
        cmds = tmp;
    }
}
