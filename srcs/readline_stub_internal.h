/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_stub_internal.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 11:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_STUB_INTERNAL_H
# define READLINE_STUB_INTERNAL_H

# ifndef MINISHELL_USE_SYSTEM_READLINE

#  include <stdlib.h>
#  include <unistd.h>
#  include <errno.h>
#  include "libft.h"
#  include "readline_wrapper.h"

typedef struct s_rl_history
{
	char				*line;
	struct s_rl_history	*next;
}	t_rl_history;

void	rl_record_history(const char *line);

# endif

#endif
