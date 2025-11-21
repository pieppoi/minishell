/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkazuhik <mkazuhik@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 05:05:00 by mkazuhik          #+#    #+#             */
/*   Updated: 2025/11/22 05:44:15 by mkazuhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define SINGLE_QUOTE_CHAR '\''
#define DOUBLE_QUOTE_CHAR '\"'

static char	*scan_single_quote(char *line)
{
	line++;
	while (*line && *line != SINGLE_QUOTE_CHAR)
		line++;
	if (*line == '\0')
	{
		print_error("minishell", NULL, "unclosed single quote");
		return (NULL);
	}
	return (line + 1);
}

static char	*scan_double_quote(char *line)
{
	line++;
	while (*line && *line != DOUBLE_QUOTE_CHAR)
		line++;
	if (*line == '\0')
	{
		print_error("minishell", NULL, "unclosed double quote");
		return (NULL);
	}
	return (line + 1);
}

static char	*scan_word_end(char *line)
{
	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			line = scan_single_quote(line);
			if (line == NULL)
				return (NULL);
		}
		else if (*line == DOUBLE_QUOTE_CHAR)
		{
			line = scan_double_quote(line);
			if (line == NULL)
				return (NULL);
		}
		else
			line++;
	}
	return (line);
}

t_token	*word(char **rest, char *line)
{
	const char	*start = line;
	char		*end;
	char		*word_str;

	end = scan_word_end(line);
	if (end == NULL)
		return (NULL);
	word_str = strndup(start, end - start);
	if (word_str == NULL)
		fatal_error("strndup");
	*rest = end;
	return (new_token(word_str, TK_WORD));
}
