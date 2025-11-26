/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:05:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/25 03:30:14 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_tokenization(char *input, t_env *env, char ***tokens_ptr)
{
	t_token	*tok;
	char	**tokens;

	errno = 0;
	tok = tokenize(input);
	if (!tok)
	{
		if (errno != EINVAL)
			print_error(NULL, NULL, "syntax error");
		g_signal = 258;
		return (-1);
	}
	expand_and_remove_quotes(tok, env);
	tokens = tokens_to_array(tok);
	free_token(tok);
	if (!tokens)
		return (-1);
	*tokens_ptr = tokens;
	return (0);
}

int	handle_tokenization_and_args(char *input, t_env **env,
		char ***tokens_ptr, char ***args_ptr)
{
	char	**tokens;

	if (process_tokenization(input, *env, &tokens) != 0)
		return (-1);
	*tokens_ptr = tokens;
	if (has_pipe(tokens))
		return (1);
	*args_ptr = extract_args(tokens);
	if (!*args_ptr || !(*args_ptr)[0])
	{
		free_array(tokens);
		if (*args_ptr)
			free(*args_ptr);
		return (-1);
	}
	return (0);
}

int	handle_redirection_and_execution(char **tokens,
		char **args, t_env **env, t_fds *fd_ptr)
{
	int	exit_status;

	if (setup_redirections(tokens, &fd_ptr->in_fd, &fd_ptr->out_fd) != 0)
		return (-1);
	fd_ptr->saved_stdin = dup(STDIN_FILENO);
	fd_ptr->saved_stdout = dup(STDOUT_FILENO);
	dup_and_close_fd(&fd_ptr->in_fd, STDIN_FILENO);
	dup_and_close_fd(&fd_ptr->out_fd, STDOUT_FILENO);
	if (is_builtin(args[0]))
		exit_status = execute_builtin(args, env);
	else
	{
		set_execution_terminal_mode();
		set_parent_execution_signals();
		exit_status = execute_external(args, env);
		set_parent_interactive_signals();
		set_interactive_terminal_mode();
	}
	return (exit_status);
}

void	cleanup_and_exit_single(char **tokens,
		char **args, int saved_stdin, int saved_stdout)
{
	restore_fds(saved_stdin, saved_stdout);
	free_array(tokens);
	free_array(args);
}

int	execute_single_command(char **tokens,
		char **args, t_env **env, t_fds *fds_ptr)
{
	int	exit_status;

	exit_status = handle_redirection_and_execution(tokens, args, env, fds_ptr);
	if (exit_status == -1)
	{
		free_array(tokens);
		free_array(args);
		if (g_signal != 130)
			g_signal = 1;
		return (-1);
	}
	if (exit_status == SHELL_EXIT_REQUEST)
	{
		cleanup_and_exit_single(tokens, args,
			fds_ptr->saved_stdin, fds_ptr->saved_stdout);
		return (SHELL_EXIT_REQUEST);
	}
	cleanup_and_exit_single(tokens, args,
		fds_ptr->saved_stdin, fds_ptr->saved_stdout);
	g_signal = exit_status;
	return (exit_status);
}
