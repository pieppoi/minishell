/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:05:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_pipe_exec_state
{
	pid_t	last_pid;
	int		start_idx;
	int		exit_code;
};

static void	restore_parent_state(void)
{
	set_parent_interactive_signals();
	set_interactive_terminal_mode();
}

static void	wait_for_remaining_children(void)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
		;
}

int	execute_pipe(char **tokens, t_env **env)
{
	t_fd_info	fdi;
	struct s_pipe_exec_state	state;

	set_execution_terminal_mode();
	set_parent_execution_signals();
	fdi.prev_pipe_read = -1;
	fdi.status = 0;
	state.start_idx = 0;
	while (1)
	{
		state.last_pid = pipe_loop_segment(tokens, env, &fdi, &state.start_idx);
		if (state.last_pid <= 0 || fdi.prev_pipe_read == -1)
			break ;
	}
	wait_for_remaining_children();
	if (state.last_pid <= 0)
	{
		restore_parent_state();
		return (1);
	}
	state.exit_code = interpret_wait_status(fdi.status, 1);
	restore_parent_state();
	return (state.exit_code);
}
