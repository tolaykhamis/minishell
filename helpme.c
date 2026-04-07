/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpme.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:51:54 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/19 16:51:54 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*getenviro(char *name, char **envp)
{
	int	i;
	int	len;

	if (!name || !envp)
		return (ft_strdup(""));
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*getval(const char *name, t_shell *shell)
{
	if (!name || name[0] == '\0')
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(shell->exit_status));
	return (getenviro((char *)name, shell->envp));
}

char	*extract_var_name(char *str, int *i)
{
	int		start;
	char	*name;

	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	name = ft_calloc((*i - start) + 1, sizeof(char));
	if (!name)
		return (NULL);
	ft_strlcpy(name, &str[start], (*i - start) + 1);
	return (name);
}

char	*append_char(char *res, char c)
{
	char	*new;
	int		len;

	len = ft_strlen(res);
	new = ft_calloc(len + 2, sizeof(char));
	if (!new)
		return (free(res), NULL);
	ft_strlcpy(new, res, len + 1);
	new[len] = c;
	return (free(res), new);
}

char	*append_str(char *res, const char *add)
{
	char	*new;
	int		len1;
	int		len2;

	len1 = ft_strlen(res);
	len2 = ft_strlen(add);
	new = ft_calloc(len1 + len2 + 1, sizeof(char));
	if (!new)
		return (free(res), NULL);
	ft_strlcpy(new, res, len1 + 1);
	ft_strlcpy(new + len1, add, len2 + 1);
	return (free(res), new);
}






