/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 04:54:10 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 07:40:29 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	set_parent_interactive_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	set_parent_execution_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	setup_signal_handlers(void)
{
	set_parent_interactive_signals();
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = 130;
	ft_putstr_fd("^C\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_signal_setting(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
