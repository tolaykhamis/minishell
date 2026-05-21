/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizerfiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:17:08 by tkhamis           #+#    #+#             */
/*   Updated: 2026/05/21 19:23:58 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void	handle_output(char **ptr, t_token **list)
{
	if (*(*ptr + 1) == '>')
	{
		add_token(list, new_token(substr_dup(">>", 2), TOKEN_APPEND));
		(*ptr) += 2;
	}
	else
	{
		add_token(list, new_token(substr_dup(">", 1), TOKEN_REDIRECT_OUT));
		(*ptr)++;
	}
}

void	handle_input(char **ptr, t_token **list)
{
	if (*(*ptr + 1) == '<')
	{
		add_token(list, new_token(substr_dup("<<", 2), TOKEN_HERDOC));
		(*ptr) += 2;
	}
	else
	{
		add_token(list, new_token(substr_dup("<", 1), TOKEN_REDIRECT_IN));
		(*ptr)++;
	}
}
