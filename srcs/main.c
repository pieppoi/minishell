/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 04:54:34 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/25 04:07:42 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// メイン関数
int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = NULL;
	configure_input_behavior();
	setup_signal_handlers();
	env = init_env(envp);
	if (!env)
	{
		ft_putendl_fd("minishell: failed to initialize environment",
			STDERR_FILENO);
		release_shell_resources(&env);
		return (1);
	}
	minishell_loop(&env);
	release_shell_resources(&env);
	return (g_signal);
}

// TTYでない場合の1行入力
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
			if (errno == EINTR)
				continue ;
			print_system_error("read", NULL);
			free(line);
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

// readlineで入力を取得
static char	*get_input_line(char *prompt)
{
	if (isatty(STDIN_FILENO))
		return (readline(prompt));
	return (read_noninteractive_line());
}

// メインループ
int	minishell_loop(t_env **env)
{
	char	*input;
	char	*prompt;

	prompt = "minishell$ ";
	while (true)
	{
		input = get_input_line(prompt);
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("\nexit\n", STDERR_FILENO);
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		parse_and_execute(input, env);
		free(input);
	}
	return (0);
}

void	parse_and_execute(char *input, t_env **env)
{
	char	**tokens;
	char	**args;
	t_fds	fds;
	int		ret;

	tokens = NULL;
	args = NULL;
	if (!input || ft_strlen(input) == 0)
		return ;
	ret = handle_tokenization_and_args(input, env, &tokens, &args);
	if (ret == -1)
		return ;
	if (ret == 1)
	{
		g_signal = execute_pipe(tokens, env);
		free_array(tokens);
		return ;
	}
	execute_single_command(tokens, args, env, &fds);
}
