/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:20:36 by tkhamis           #+#    #+#             */
/*   Updated: 2026/04/25 17:17:37 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	freeargs(t_cmdlist *cmds)
{
	t_cmdlist	*tmp;
	int			i;

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

void	free_free(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	// arr = NULL;
}

void	free_redirs(t_redi *redi)
{
	t_redi	*tmp;

	while (redi)
	{
		tmp = redi->next;
		if (redi->f)
			free(redi->f);
		free(redi);
		redi = tmp;
	}
}
