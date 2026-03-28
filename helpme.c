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
void	freeeeeeeeeeeeeee(t_cmdlist *cmds)
{
	t_cmdlist	*tmp_cmd;
	t_redi		*tmp_redi;
	int			i;

	while (cmds)
	{
		tmp_cmd = cmds->next;
		if (cmds->av)
		{
			i = 0;
			while (cmds->av[i])
				free(cmds->av[i++]);
			free(cmds->av);
		}
		while (cmds->redirs)
		{
			tmp_redi = cmds->redirs->next;
			if (cmds->redirs->f)
				free(cmds->redirs->f);
			free(cmds->redirs);
			cmds->redirs = tmp_redi;
		}
		free(cmds);
		cmds = tmp_cmd;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;
	size_t	i;

	total = nmemb * size;
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < total)
	{
		((unsigned char *)ptr)[i] = 0;
		i++;
	}
	return (ptr);
}



char	*ft_strdup(const char *s1)
{
	char	*dest;
	size_t	i = 0;

	dest = (char *)malloc(ft_strlen(s1) + 1);
	if (!dest)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] && i < (size - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

int	ft_isalnum(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

static int	get_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	long	num;

	num = n;
	len = get_len(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (num == 0)
		str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		str[len--] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}

void free_redirs(t_redi *redi)
{
    t_redi *tmp;

    while (redi)
    {
        tmp = redi->next;
        if (redi->f)
            free(redi->f);
        free(redi);
        redi = tmp;
    }
}

int	ft_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}
