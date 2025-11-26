/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 08:00:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:00:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	release_shell_resources(t_env **env)
{
	if (env && *env)
	{
		free_env_list(*env);
		*env = NULL;
	}
	rl_clear_history();
	restore_terminal_behavior();
}

void	shutdown_shell(t_env **env, int exit_code)
{
	release_shell_resources(env);
	exit(exit_code);
}
