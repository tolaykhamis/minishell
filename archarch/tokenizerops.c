/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizerops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkhamis <tkhamis@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:19:24 by tkhamis           #+#    #+#             */
/*   Updated: 2026/03/19 16:19:24 by tkhamis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipes(char **ptr, t_token **list)
{
	if (*(*ptr + 1) == '|')
	{
		add_token(list, new_token(substr_dup("||", 2), TOKEN_OR));
		(*ptr) += 2;
	}
	else
	{
		add_token(list, new_token(substr_dup("|", 1), TOKENPIPE));
		(*ptr)++;
	}
}

void	handle_operator(char **ptr, t_token **list)
{
	if (**ptr == '|')
		pipes(ptr, list);
	else if (**ptr == '&')
	{
		if (*(*ptr + 1) == '&')
		{
			add_token(list, new_token(substr_dup("&&", 2), TOKEN_AND));
			(*ptr) += 2;
		}
		else
		{
			while (**ptr)
				(*ptr)++;
		}
	}
	else if (**ptr == '>')
		handle_output(ptr, list);
	else if (**ptr == '<')
		handle_input(ptr, list);
}
