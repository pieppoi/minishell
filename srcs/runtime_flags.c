/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime_flags.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 08:50:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 08:50:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifdef IUTF8

void	apply_utf8_flags(struct termios *interactive, struct termios *execution)
{
	interactive->c_iflag |= IUTF8;
	execution->c_iflag |= IUTF8;
}
#else

void	apply_utf8_flags(struct termios *interactive, struct termios *execution)
{
	(void)interactive;
	(void)execution;
}
#endif

#ifdef ECHOCTL

void	apply_echoctl_flags(struct termios *interactive,
		struct termios *execution)
{
	interactive->c_lflag &= ~ECHOCTL;
	execution->c_lflag |= ECHOCTL;
}
#else

void	apply_echoctl_flags(struct termios *interactive,
		struct termios *execution)
{
	(void)interactive;
	(void)execution;
}
#endif
