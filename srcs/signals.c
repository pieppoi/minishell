/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 04:54:10 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/26 23:06:43 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// グローバル変数（シグナル用）
int	g_signal = 0;

// 親プロセスのシグナルハンドラー設定
void	set_parent_interactive_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	set_parent_execution_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

// シグナルハンドラーを設定（親プロセス初期化用）
void	setup_signal_handlers(void)
{
	set_parent_interactive_signals();
}

// SIGINT ハンドラー (Ctrl+C)
void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = 130;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// 子プロセス用のシグナル設定（パイプ実行時）
void	child_signal_setting(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
