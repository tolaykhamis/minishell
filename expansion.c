/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:33 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/07 22:52:33 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*quote_removal_part_100(char *str)
{
	char	*new;
	int		i = 0, j = 0, sq = 0, dq = 0;

	if (!str)
		return (NULL);
	new = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !dq)
			sq = !sq;
		else if (str[i] == '\"' && !sq)
			dq = !dq;
		else
			new[j++] = str[i];
		i++;
	}
	free(str);
	return (new);
}

static char *handle_dollar(char *str, int *i, char *res, t_shell *shell)
{
	char	*name;
	char	*value;

	(*i)++;
	if (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_' || str[*i] == '?'))
	{
		name = extract_var_name(str, i);
		if (!name)
			return (res);
		value = getval(name, shell);
		free(name);
		res = append_str(res, value);
		free(value);
	}
	else
		res = append_char(res, '$');
	return (res);
}

char	*expand_parameter(char *str, t_shell *shell)
{
	char	*res;
	int		i = 0, sq = 0, dq = 0;

	if (!str)
		return (NULL);
	res = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\"' && !sq)
			dq = !dq;
		else if (str[i] == '\'' && !dq)
			sq = !sq;
		if (str[i] == '$' && !sq)
			res = handle_dollar(str, &i, res, shell);
		else
			res = append_char(res, str[i++]);
	}
	return (res);
}

static char **takeword(char *str, t_shell *shell)
{
	char	*prevexp;
	char	**split_res;
	int		had_quotes;

	had_quotes = (ft_strchr(str, '\'') || ft_strchr(str, '\"'));
	prevexp = expand_parameter(str, shell);
	if (prevexp && prevexp[0] == '\0' && !had_quotes)
	{
		free(prevexp);
		return (NULL);
	}
	if (prevexp && !had_quotes && ft_strchr(prevexp, ' '))
	{
		split_res = ft_split(prevexp, ' ');
		free(prevexp);
	}
	else
	{
		split_res = ft_calloc(2, sizeof(char *));
		if (had_quotes)
			split_res[0] = quote_removal_part_100(prevexp);
		else
			split_res[0] = prevexp;
	}
	return (split_res);
}

static int countsizeofexp(char **av, t_shell *shell)
{
	int		i = 0, count = 0, k;
	char	**split;

	while (av && av[i])
	{
		split = takeword(av[i], shell);
		if (split)
		{
			k = 0;
			while (split[k])
			{
				free(split[k++]);
				count++;
			}
			free(split);
		}
		i++;
	}
	return (count);
}
static char	**handle_expansion(char **old_av, int size, t_shell *shell)
{
	char	**new_av;
	char	**split;
	int		i;
	int		j;
	int		k;

	new_av = ft_calloc(size + 1, sizeof(char *));
	if (!new_av)
		return (NULL);
	i = 0;
	j = 0;
	while (old_av && old_av[i])
	{
		split = takeword(old_av[i], shell);
		k = 0;
		while (split && split[k])
			new_av[j++] = split[k++];
		free(split);
		free(old_av[i++]);
	}
	return (new_av);
}

static void	expargs(t_cmdlist *cmd, t_shell *shell)
{
	int		size;
	int		i;

	size = countsizeofexp(cmd->av, shell);
	if (size == 0)
	{
		i = 0;
		while (cmd->av && cmd->av[i])
			free(cmd->av[i++]);
		free(cmd->av);
		cmd->av = NULL;
		return ;
	}
	cmd->av = handle_expansion(cmd->av, size, shell);
}

static void emptyrmv(t_cmdlist **cmds)
{
	t_cmdlist *prev = NULL;
	t_cmdlist *curr = *cmds;
	t_cmdlist *temp;

	while (curr)
	{
		if ((!curr->av || !curr->av[0]) && !curr->redirs)
		{
			temp = curr;
			if (prev)
				prev->next = curr->next;
			else
				*cmds = curr->next;
			curr = curr->next;
			free(temp->av);
			free(temp);
			continue;
		}
		prev = curr;
		curr = curr->next;
	}
}

void expander(t_cmdlist **cmds, t_shell *shell)
{
	t_cmdlist	*curr;
	t_redi		*rd;
	char		*tmp;

	if (!cmds || !*cmds)
		return ;
	curr = *cmds;
	while (curr)
	{
		expargs(curr, shell);
		rd = curr->redirs;
		while (rd)
		{
			if (rd->type != TOKEN_HERDOC)
			{
				tmp = rd->f;
				rd->f = expand_parameter(tmp, shell);
				free(tmp);
			}
			rd->f = quote_removal_part_100(rd->f);
			rd = rd->next;
		}
		curr = curr->next;
	}
	emptyrmv(cmds);
}
