/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:56 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:03:57 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_isbuiltin(t_cmdlist *cmd)
{
	if (!cmd->av || !cmd->av[0])
		return ;
	if (ft_strcmp(cmd->av[0], "echo") == 0 || ft_strcmp(cmd->av[0], "pwd") == 0
		|| ft_strcmp(cmd->av[0], "cd") == 0 || ft_strcmp(cmd->av[0],
			"exit") == 0 || ft_strcmp(cmd->av[0], "env") == 0
		|| ft_strcmp(cmd->av[0], "export") == 0 || ft_strcmp(cmd->av[0],
			"unset") == 0)
		cmd->is_builtin = 1;
	else
		cmd->is_builtin = 0;
}

void	ft_swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	yes_value(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}
