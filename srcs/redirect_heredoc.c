/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    ft +:+         +:+      */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:25:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/22 05:25:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_warning(char *delimiter)
{
	ft_putendl_fd("", STDOUT_FILENO);
	ft_putstr_fd("minishell: warning: here-document delimited by end-of-file",
		STDERR_FILENO);
	ft_putstr_fd(" (wanted '", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

static void	run_heredoc_child(char *delimiter, int write_fd)
{
	char	*line;
	char	*prompt;

	prompt = "> ";
	while (1)
	{
		line = readline(prompt);
		if (!line)
		{
			print_heredoc_warning(delimiter);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, write_fd);
		free(line);
	}
	close(write_fd);
}

static int	wait_heredoc_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
		return (-1);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		wait_status;

	if (!delimiter)
		return (-1);
	if (pipe(pipe_fd) < 0)
	{
		print_error("pipe", NULL, strerror(errno));
		return (-1);
	}
	set_parent_execution_signals();
	pid = fork();
	if (pid < 0)
	{
		print_error("fork", NULL, strerror(errno));
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		set_parent_interactive_signals();
		return (-1);
	}
	if (pid == 0)
	{
		child_signal_setting();
		close(pipe_fd[0]);
		run_heredoc_child(delimiter, pipe_fd[1]);
		_exit(0);
	}
	close(pipe_fd[1]);
	wait_status = wait_heredoc_child(pid);
	set_parent_interactive_signals();
	if (wait_status == 130)
	{
		g_signal = 130;
		close(pipe_fd[0]);
		return (-1);
	}
	if (wait_status != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
