/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_stub.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:30:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/27 10:30:00 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_USE_SYSTEM_READLINE

# include "readline_stub_internal.h"

static void	display_prompt(const char *prompt)
{
	if (!prompt)
		return ;
	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
}

static int	append_char(char **line, char ch)
{
	char	*new_line;
	size_t	len;

	len = ft_strlen(*line);
	new_line = (char *)malloc(len + 2);
	if (!new_line)
	{
		free(*line);
		*line = NULL;
		return (-1);
	}
	if (*line)
		ft_strlcpy(new_line, *line, len + 1);
	new_line[len] = ch;
	new_line[len + 1] = '\0';
	free(*line);
	*line = new_line;
	return (0);
}

static char	*handle_line_completion(char **line)
{
	if (!*line)
		return (ft_strdup(""));
	rl_record_history(*line);
	return (*line);
}

static char	*finalize_line(char **line, ssize_t bytes)
{
	if (bytes == 0)
		return (*line);
	free(*line);
	*line = NULL;
	return (NULL);
}

char	*readline(const char *prompt)
{
	char	*line;
	char	ch;
	ssize_t	bytes;

	line = NULL;
	display_prompt(prompt);
	while (1)
	{
		bytes = read(STDIN_FILENO, &ch, 1);
		if (bytes == -1 && errno == EINTR)
			continue ;
		if (bytes <= 0)
			return (finalize_line(&line, bytes));
		if (ch == '\n')
			return (handle_line_completion(&line));
		if (append_char(&line, ch) != 0)
			return (NULL);
	}
}

#endif
