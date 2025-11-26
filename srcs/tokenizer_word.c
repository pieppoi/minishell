/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:05:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/25 03:30:14 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define SINGLE_QUOTE_CHAR '\''
#define DOUBLE_QUOTE_CHAR '\"'

static void	report_unclosed_quote(bool is_single)
{
	if (is_single)
		print_error("minishell", NULL, "unclosed single quote");
	else
		print_error("minishell", NULL, "unclosed double quote");
}

static int	skip_quoted(char **line_ptr, char quote_char)
{
	char	*cursor;

	cursor = *line_ptr + 1;
	while (*cursor && *cursor != quote_char)
		cursor++;
	if (*cursor == '\0')
		return (-1);
	*line_ptr = cursor + 1;
	return (0);
}

static int	handle_word_quoted_segment(char **line_ptr, char quote_char,
		bool *error_ptr)
{
	if (skip_quoted(line_ptr, quote_char) != 0)
	{
		if (error_ptr)
			*error_ptr = true;
		report_unclosed_quote(quote_char == SINGLE_QUOTE_CHAR);
		return (-1);
	}
	return (0);
}

static char	*scan_word_end(char *line, bool *error_ptr)
{
	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR || *line == DOUBLE_QUOTE_CHAR)
		{
			if (handle_word_quoted_segment(&line, *line, error_ptr) != 0)
				return (NULL);
			continue ;
		}
		line++;
	}
	return (line);
}

t_token	*word(char **rest, char *line, bool *error_ptr)
{
	const char	*start;
	char		*end;
	char		*word_str;

	start = line;
	end = scan_word_end(line, error_ptr);
	if (!end)
		return (NULL);
	word_str = ft_substr(start, 0, end - start);
	if (word_str == NULL)
		fatal_error("ft_substr");
	if (rest)
		*rest = end;
	return (new_token(word_str, TK_WORD));
}
