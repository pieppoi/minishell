/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 04:54:10 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:00:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <locale.h>

static struct termios	g_original_termios;
static struct termios	g_interactive_termios;
static struct termios	g_execution_termios;
static bool	g_termios_saved;

static void	initialize_termios_modes(void)
{
	struct termios	current;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &current) == -1)
		return ;
	g_original_termios = current;
	g_interactive_termios = current;
	g_execution_termios = current;
	g_termios_saved = true;
	apply_utf8_flags(&g_interactive_termios, &g_execution_termios);
	apply_echoctl_flags(&g_interactive_termios, &g_execution_termios);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_interactive_termios);
}

void	set_interactive_terminal_mode(void)
{
	if (!g_termios_saved || !isatty(STDIN_FILENO))
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, &g_interactive_termios);
}

void	set_execution_terminal_mode(void)
{
	if (!g_termios_saved || !isatty(STDIN_FILENO))
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, &g_execution_termios);
}

void	configure_input_behavior(void)
{
	setlocale(LC_CTYPE, "");
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	rl_variable_bind("convert-meta", "off");
	rl_variable_bind("input-meta", "on");
	rl_variable_bind("output-meta", "on");
	initialize_termios_modes();
}

void	restore_terminal_behavior(void)
{
	if (!g_termios_saved || !isatty(STDIN_FILENO))
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, &g_original_termios);
}
