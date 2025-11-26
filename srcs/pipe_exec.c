/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:05:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/22 05:05:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_and_reset_fd(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

static void	reset_fd_info_streams(t_fd_info *fdi_ptr)
{
	close_and_reset_fd(&fdi_ptr->in_fd);
	close_and_reset_fd(&fdi_ptr->out_fd);
	close_and_reset_fd(&fdi_ptr->prev_pipe_read);
}

static pid_t	handle_fatal_redirection_failure(t_fd_info *fdi_ptr, int has_next)
{
	reset_fd_info_streams(fdi_ptr);
	if (has_next)
	{
		close(fdi_ptr->pipe_fd[0]);
		close(fdi_ptr->pipe_fd[1]);
	}
	return (-1);
}

static pid_t	handle_nonfatal_redirection_failure(t_fd_info *fdi_ptr,
		int has_next, t_token_range range, int *start_idx_ptr)
{
	reset_fd_info_streams(fdi_ptr);
	if (has_next)
	{
		close(fdi_ptr->pipe_fd[1]);
		fdi_ptr->prev_pipe_read = fdi_ptr->pipe_fd[0];
	}
	else
	{
		fdi_ptr->prev_pipe_read = -1;
		fdi_ptr->status = (1 & 0xff) << 8;
	}
	if (has_next)
		*start_idx_ptr = range.end_idx + 1;
	else
		*start_idx_ptr = range.end_idx;
	return (1);
}

void	cleanup_parent_fds_and_pipe(t_fd_info *fdi_ptr, int has_next)
{
	if (fdi_ptr->in_fd >= 0)
		close(fdi_ptr->in_fd);
	if (fdi_ptr->out_fd >= 0)
		close(fdi_ptr->out_fd);
	if (fdi_ptr->prev_pipe_read >= 0)
		close(fdi_ptr->prev_pipe_read);
	if (has_next)
	{
		close(fdi_ptr->pipe_fd[1]);
		fdi_ptr->prev_pipe_read = fdi_ptr->pipe_fd[0];
	}
	else
		fdi_ptr->prev_pipe_read = -1;
}

static int	setup_pipe_if_needed(t_fd_info *fdi_ptr, int has_next)
{
	if (has_next)
	{
		if (pipe(fdi_ptr->pipe_fd) < 0)
		{
			print_error("pipe", NULL, strerror(errno));
			return (-1);
		}
	}
	return (0);
}

static pid_t	execute_segment_process(char **args,
		t_env **env, t_fd_info *fdi_ptr, int has_next)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_child_process_pipe(args, env, *fdi_ptr, has_next);
	cleanup_parent_fds_and_pipe(fdi_ptr, has_next);
	if (!has_next)
	{
		waitpid(pid, &fdi_ptr->status, 0);
		free_array(args);
		return (pid);
	}
	free_array(args);
	return (pid);
}

pid_t	pipe_loop_segment(char **tokens,
		t_env **env, t_fd_info *fdi_ptr, int *start_idx_ptr)
{
	char			**args;
	t_token_range	range;
	int				has_next;
	t_pipe_redir_status	redir_status;

	find_pipe_segment(tokens, start_idx_ptr, &range, &has_next);
	if (setup_pipe_if_needed(fdi_ptr, has_next) != 0)
		return (-1);
	redir_status = setup_pipe_redirections(tokens, range,
			&fdi_ptr->in_fd, &fdi_ptr->out_fd);
	if (redir_status == PIPE_REDIR_ERR_FATAL)
		return (handle_fatal_redirection_failure(fdi_ptr, has_next));
	if (redir_status == PIPE_REDIR_ERR_NON_FATAL)
		return (handle_nonfatal_redirection_failure(
				fdi_ptr, has_next, range, start_idx_ptr));
	args = get_pipe_command(tokens, start_idx_ptr);
	if (!args || !args[0])
	{
		if (has_next)
		{
			close(fdi_ptr->pipe_fd[0]);
			close(fdi_ptr->pipe_fd[1]);
		}
		return (0);
	}
	return (execute_segment_process(args, env, fdi_ptr, has_next));
}

int	execute_pipe(char **tokens, t_env **env)
{
	t_fd_info		fdi;
	int				status;
	int				start_idx;
	pid_t			last_pid;
	int				exit_code;

	set_execution_terminal_mode();
	set_parent_execution_signals();
	fdi.prev_pipe_read = -1;
	fdi.status = 0;
	start_idx = 0;
	while (1)
	{
		last_pid = pipe_loop_segment(tokens, env, &fdi, &start_idx);
		if (last_pid <= 0)
			break ;
		if (fdi.prev_pipe_read == -1)
			break ;
	}
	while (waitpid(-1, &status, 0) > 0)
		;
	if (last_pid <= 0)
	{
		set_parent_interactive_signals();
		set_interactive_terminal_mode();
		return (1);
	}
	exit_code = interpret_wait_status(fdi.status, 1);
	set_parent_interactive_signals();
	set_interactive_terminal_mode();
	return (exit_code);
}
