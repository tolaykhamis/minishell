/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorhandler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:16 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/07 22:52:16 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	outputerror(const char *token)
{
	printf("minishell: syntax error near unexpected token `%s`\n", token);
	return (0);
}

int	valoftoken(t_token *t)
{
	if (oper(t->type))
	{
		if (!t->next)
			return (outputerror("newline"));
		if (oper(t->next->type))
			return (outputerror(convert(t->next->type)));
	}
	if (redi(t->type))
	{
		if (!t->next || t->next->type != TOKEN_WORD)
			return (outputerror("newline"));
		if (t->next->type != TOKEN_WORD)
			return (outputerror(convert(t->next->type)));
	}
	return (0);
}

int	check_syntax(t_token *tokens)
{
	t_token	*t;
	int		err;

	if (!tokens)
		return (1);
	if (oper(tokens->type))
		return (outputerror(convert(tokens->type)));
	t = tokens;
	while (t)
	{
		if (redi(t->type))
		{
			if (!t->next || t->next->type != TOKEN_WORD)
				return (outputerror("newline"));
		}
		err = valoftoken(t);
		if (err != 0)
			return (err);
		t = t->next;
	}
	return (1);
}
