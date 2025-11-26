/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_stub_history.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 11:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_USE_SYSTEM_READLINE

# include "readline_stub_internal.h"

static t_rl_history	*g_rl_history;

static t_rl_history	*new_history_node(const char *line)
{
	t_rl_history	*node;

	if (!line || !*line)
		return (NULL);
	node = (t_rl_history *)malloc(sizeof(t_rl_history));
	if (!node)
		return (NULL);
	node->line = ft_strdup(line);
	if (!node->line)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	rl_record_history(const char *line)
{
	t_rl_history	*node;

	node = new_history_node(line);
	if (!node)
		return ;
	node->next = g_rl_history;
	g_rl_history = node;
}

void	add_history(const char *line)
{
	rl_record_history(line);
}

void	rl_clear_history(void)
{
	t_rl_history	*next;

	while (g_rl_history)
	{
		next = g_rl_history->next;
		free(g_rl_history->line);
		free(g_rl_history);
		g_rl_history = next;
	}
}

#endif
