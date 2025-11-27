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

# ifndef MINISHELL_USE_SYSTEM_READLINE
#  error "MINISHELL_USE_SYSTEM_READLINE must be defined to build without stubs"
# endif

# include <readline/readline.h>
# include <readline/history.h>

# define RL_CATCH_SIGNALS rl_catch_signals
# define RL_CATCH_SIGWINCH rl_catch_sigwinch

#endif
