/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomeltah <jomeltah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:04:50 by jomeltah          #+#    #+#             */
/*   Updated: 2026/04/11 16:04:51 by jomeltah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_signal = 0;

static void	handle_sigint(int sig)
{
	(void)sig;
	// if(g_signal == 130)
	// {
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// rl_redisplay();
	// }
	g_signal = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// void *handler_hala(void)
// {
// 	g_signal = 130;
// 	signal(SIGINT, SIG_DFL);
// 	return ;
// }
void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
	}
}
void	signal_handler_q(int signal)
{
	if (signal == SIGQUIT)
	{
		g_signal = SIGQUIT;
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
}

void	setup_signals_child(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler_q);
}
static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}
void setup_signals_heredoc(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
