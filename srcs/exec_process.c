/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:25:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/25 03:33:23 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child_process(char *exec_path, char **argv, t_env **env)
{
	char	**envp;
	int		exit_code;

	child_signal_setting();
	envp = env_to_array(*env);
	if (!envp)
		_exit(1);
	execve(exec_path, argv, envp);
	print_system_error(argv[0], NULL);
	free_array(envp);
	if (errno == ENOENT)
		exit_code = 127;
	else
		exit_code = 126;
	_exit(exit_code);
}

int	interpret_wait_status(int status, int print_signal_msg)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (print_signal_msg)
		{
			if (WTERMSIG(status) == SIGINT)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putendl_fd("Quit: 3", STDERR_FILENO);
		}
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
		return (1);
	return (interpret_wait_status(status, 1));
}

int	execute_external(char **argv, t_env **env)
{
	pid_t	pid;
	char	*exec_path;
	int		exit_status;

	if (!argv || !argv[0])
		return (1);
	exec_path = find_executable(argv[0], *env);
	if (!exec_path)
	{
		print_error(argv[0], NULL, "command not found");
		return (127);
	}
	pid = fork();
	if (pid < 0)
	{
		free(exec_path);
		print_system_error(argv[0], NULL);
		return (1);
	}
	if (pid == 0)
		execute_child_process(exec_path, argv, env);
	free(exec_path);
	exit_status = wait_for_child(pid);
	return (exit_status);
}
