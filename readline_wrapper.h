/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_wrapper.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 10:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_WRAPPER_H
# define READLINE_WRAPPER_H

# ifdef MINISHELL_USE_SYSTEM_READLINE
#  include <readline/readline.h>
#  include <readline/history.h>
#  define RL_CATCH_SIGNALS rl_catch_signals
#  define RL_CATCH_SIGWINCH rl_catch_sigwinch
# else

char	*readline(const char *prompt);
void	add_history(const char *line);
void	rl_clear_history(void);
void	rl_on_new_line(void);
void	rl_replace_line(const char *text, int clear_undo);
void	rl_redisplay(void);
int		rl_variable_bind(const char *name, const char *value);
extern int	g_rl_catch_signals;
extern int	g_rl_catch_sigwinch;
#  define RL_CATCH_SIGNALS g_rl_catch_signals
#  define RL_CATCH_SIGWINCH g_rl_catch_sigwinch
# endif

#endif
