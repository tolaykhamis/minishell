/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpersforerror.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:38:48 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/19 16:38:48 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*convert(enum token type)
{
	if (type == TOKENPIPE) return ("|");
	if (type == TOKEN_OR) return ("||");
	if (type == TOKEN_AND) return ("&&");
	if (type == TOKEN_REDIRECT_IN) return ("<");
	if (type == TOKEN_REDIRECT_OUT) return (">");
	if (type == TOKEN_APPEND) return (">>");
	if (type == TOKEN_HERDOC) return ("<<");
	return ("unknown");
}

int	redi(enum token type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT 
			|| type == TOKEN_APPEND || type == TOKEN_HERDOC);
}

int	oper(enum token type)
{
	return (type == TOKENPIPE || type == TOKEN_OR || type == TOKEN_AND);
}