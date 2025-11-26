/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 04:54:44 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/23 21:11:47 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_exit_value(char *arg, long long *result)
{
	char		*endptr;
	long long	value;

	if (!arg || !result)
		return (0);
	errno = 0;
	value = strtoll(arg, &endptr, 10);
	if (errno == ERANGE || *endptr != '\0')
		return (0);
	*result = value;
	return (1);
}

// exit コマンドの実装
int	ft_exit(char **args, t_env **env)
{
	long long	exit_value;

	(void)env;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args || !args[1])
	{
		g_signal = 0;
		return (SHELL_EXIT_REQUEST);
	}
	if (args[2])
	{
		print_error("exit", NULL, "too many arguments");
		g_signal = 1;
		return (1);
	}
	if (!parse_exit_value(args[1], &exit_value))
	{
		print_error("exit", args[1], "numeric argument required");
		g_signal = 2;
		return (SHELL_EXIT_REQUEST);
	}
	g_signal = (unsigned char)exit_value;
	return (SHELL_EXIT_REQUEST);
}
