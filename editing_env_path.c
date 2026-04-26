/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editing_env_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:07 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/26 18:50:05 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**remove_from_envp(char **envp, char *name)
{
	int	i;
	int	j;
	int	len;

	if (!envp || !name)
		return (envp);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0)
		{
			free(envp[i]);
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			return (envp);
		}
		i++;
	}
	return (envp);
}

static int	update_existing(char **envp, char *new)
{
	int	i;
	int	len;
	int len2;

	i = 0;
	len = 0;
	while (new[len] && new[len] != '=')
		len++;
	while (envp[i])
	{
		len2 = 0;
		while (envp[i][len2] && envp[i][len2] != '=')
			len2++;
		if (!ft_strncmp(envp[i], new, len) && (len2 ==len))
		{
			free(envp[i]);
			envp[i] = ft_strdup(new);
			return (1);
		}
		i++;
	}
	return (0);
}

static char	**add_new(char **envp, char *new)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = ft_calloc(i + 2, sizeof(char *));
	if (!new_envp)
		return (envp);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]); //delete this
		i++;
	}
	new_envp[i] = ft_strdup(new);
	new_envp[i + 1] = NULL;
	free_free(envp);
	return (new_envp);
}

char	**add_to_envp(char **envp, char *new)
{
	if (!envp || !new)
		return (envp);
	if (update_existing(envp, new))
		return (envp);
	return (add_new(envp, new));
}

char	*get_env_name(char **envp, char *name)
{
	int	i;
	int	len;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
