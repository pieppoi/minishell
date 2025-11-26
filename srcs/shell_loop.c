/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 09:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 09:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_read_failure(char **line)
{
	if (errno == EINTR)
		return (1);
	print_system_error("read", NULL);
	free(*line);
	*line = NULL;
	return (0);
}

static char	*read_noninteractive_line(void)
{
	char	*line;
	char	ch;
	ssize_t	bytes;

	line = NULL;
	while (1)
	{
		bytes = read(STDIN_FILENO, &ch, 1);
		if (bytes == -1)
		{
			if (handle_read_failure(&line))
				continue ;
			return (NULL);
		}
		if (bytes == 0)
			break ;
		if (ch == '\n')
		{
			if (!line)
				return (ft_strdup(""));
			return (line);
		}
		append_char(&line, ch);
	}
	return (line);
}

static char	*get_input_line(char *prompt)
{
	static int	prompt_printed;

	if (isatty(STDIN_FILENO))
		return (readline(prompt));
	if (!prompt_printed && prompt)
	{
		ft_putstr_fd(prompt, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		prompt_printed = 1;
	}
	return (read_noninteractive_line());
}

int	minishell_loop(t_env **env)
{
	char	*input;
	int		exec_status;

	while (true)
	{
		input = get_input_line("minishell$ ");
		if (!input)
			break ;
		if (ft_strlen(input) > 0)
			add_history(input);
		exec_status = parse_and_execute(input, env);
		if (exec_status == SHELL_EXIT_REQUEST)
		{
			free(input);
			shutdown_shell(env, g_signal);
		}
		free(input);
	}
	if (!input && isatty(STDIN_FILENO))
		ft_putstr_fd("\nexit\n", STDERR_FILENO);
	return (0);
}

int	parse_and_execute(char *input, t_env **env)
{
	char	**tokens;
	char	**args;
	t_fds	fds;
	int		ret;
	int		exec_status;

	if (!input || ft_strlen(input) == 0)
		return (0);
	ret = handle_tokenization_and_args(input, env, &tokens, &args);
	if (ret == -1)
		return (0);
	if (ret == 1)
	{
		g_signal = execute_pipe(tokens, env);
		free_array(tokens);
		return (0);
	}
	exec_status = execute_single_command(tokens, args, env, &fds);
	if (exec_status == SHELL_EXIT_REQUEST)
		return (SHELL_EXIT_REQUEST);
	return (0);
}
