/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 08:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:30:00 by mkazuhik         ###   ########.fr       */
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

pid_t	handle_fatal_redirection_failure(t_fd_info *fdi_ptr, int has_next)
{
	reset_fd_info_streams(fdi_ptr);
	if (has_next)
	{
		close(fdi_ptr->pipe_fd[0]);
		close(fdi_ptr->pipe_fd[1]);
	}
	return (-1);
}

pid_t	handle_nonfatal_redirection_failure(t_fd_info *fdi_ptr,
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
