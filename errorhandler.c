/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorhandler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 22:52:16 by tkhamis           #+#    #+#             */
/*   Updated: 2026/04/27 16:13:05 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	outputerror(const char *token)
{
	printf("minishell: syntax error near unexpected token `%s`\n", token);
	return (1);
}

int	valoftoken(t_token *t)
{
	if (t->type == TOKEN_OR || t->type == TOKEN_AND)
		return (outputerror(convert(t->type)));
	if (oper(t->type))
	{
		if (!t->next)
			return (outputerror("newline"));
		if (oper(t->next->type))
			return (outputerror(convert(t->next->type)));
	}
	if (redi(t->type))
	{
		if (!t->next)
			return (outputerror("newline"));
		if (t->next->type != TOKEN_WORD)
			return (outputerror(convert(t->next->type)));
	}
	return (0);
}

int	check_syntax(t_token *tokens)
{
	t_token	*t;

	if (!tokens)
		return (1);
	if (oper(tokens->type))
	{
		outputerror(convert(tokens->type));
		return (0);
	}
	t = tokens;
	while (t)
	{
		if (valoftoken(t) != 0)
			return (0);
		t = t->next;
	}
	return (1);
}
