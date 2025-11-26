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

static int	init_shell_environment(t_env **env, char **envp)
{
	*env = init_env(envp);
	if (!*env)
	{
		ft_putendl_fd("minishell: failed to initialize environment",
			STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = NULL;
	configure_input_behavior();
	setup_signal_handlers();
	if (!init_shell_environment(&env, envp))
	{
		release_shell_resources(&env);
		return (1);
	}
	minishell_loop(&env);
	release_shell_resources(&env);
	return (g_signal);
}
