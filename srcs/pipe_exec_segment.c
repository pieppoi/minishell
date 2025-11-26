/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec_segment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 08:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	handle_fatal_redirection_failure(t_fd_info *fdi_ptr, int has_next);
pid_t	handle_nonfatal_redirection_failure(t_fd_info *fdi_ptr, int has_next,
			t_token_range range, int *start_idx_ptr);

static int	setup_pipe_if_needed(t_fd_info *fdi_ptr, int has_next)
{
	if (!has_next)
		return (0);
	if (pipe(fdi_ptr->pipe_fd) < 0)
	{
		print_error("pipe", NULL, strerror(errno));
		return (-1);
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

static pid_t	handle_missing_args(t_fd_info *fdi_ptr, int has_next)
{
	if (has_next)
	{
		close(fdi_ptr->pipe_fd[0]);
		close(fdi_ptr->pipe_fd[1]);
	}
	return (0);
}

pid_t	pipe_loop_segment(char **tokens,
		t_env **env, t_fd_info *fdi_ptr, int *start_idx_ptr)
{
	char	**args;
	t_token_range	range;
	int	has_next;
	t_pipe_redir_status	status;

	find_pipe_segment(tokens, start_idx_ptr, &range, &has_next);
	if (setup_pipe_if_needed(fdi_ptr, has_next) != 0)
		return (-1);
	status = setup_pipe_redirections(tokens, range,
			&fdi_ptr->in_fd, &fdi_ptr->out_fd);
	if (status == PIPE_REDIR_ERR_FATAL)
		return (handle_fatal_redirection_failure(fdi_ptr, has_next));
	if (status == PIPE_REDIR_ERR_NON_FATAL)
		return (handle_nonfatal_redirection_failure(
				fdi_ptr, has_next, range, start_idx_ptr));
	args = get_pipe_command(tokens, start_idx_ptr);
	if (!args || !args[0])
		return (handle_missing_args(fdi_ptr, has_next));
	return (execute_segment_process(args, env, fdi_ptr, has_next));
}
