/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_stub_display.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 11:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_USE_SYSTEM_READLINE

# include "readline_stub_internal.h"

int	g_rl_catch_signals = 0;
int	g_rl_catch_sigwinch = 0;

int	rl_variable_bind(const char *name, const char *value)
{
	(void)name;
	(void)value;
	return (0);
}

void	rl_on_new_line(void)
{
}

void	rl_replace_line(const char *text, int clear_undo)
{
	(void)text;
	(void)clear_undo;
}

void	rl_redisplay(void)
{
}

#endif
