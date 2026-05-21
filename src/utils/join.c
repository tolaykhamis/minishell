/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:01 by jomeltah          #+#    #+#             */
/*   Updated: 2026/05/21 19:23:58 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


static void	no_space(char *s, size_t len1, char *s1, char *s2)
{
	size_t	i;
	int		k;
	int		m;
	size_t	j;
	size_t	len2;

	len2 = ft_strlen(s2);
	i = 0;
	k = 0;
	m = 0;
	j = 0;
	while (i < len1)
		s[i++] = s1[k++];
	j = 0;
	while (j < len2)
	{
		s[i] = s2[m];
		j++;
		i++;
		m++;
	}
	s[i] = '\0';
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*s;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (!s1 && !s2)
		return (NULL);
	s = malloc(len1 + len2 + 1);
	if (!s)
		return (NULL);
	no_space(s, len1, s1, s2);
	return (s);
}
