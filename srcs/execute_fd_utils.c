/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 08:45:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:45:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_and_close_fd(int *fd_ptr, int target)
{
	if (*fd_ptr < 0)
		return ;
	dup2(*fd_ptr, target);
	close(*fd_ptr);
	*fd_ptr = -1;
}
