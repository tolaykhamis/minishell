/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:03:25 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:03:26 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *quote_removal_part_100(char *str)
{
        char    *new;
        int             i = 0, j = 0, sq = 0, dq = 0;

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
        char    *name;
        char    *value;

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



char    *expand_parameter(char *str, t_shell *shell)
{
        char    *res;
        int             i = 0, sq = 0, dq = 0;

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
char **takeword(char *str, t_shell *shell)
{
        char    *prevexp;
        char    **split_res;
        int             had_quotes;

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
    
