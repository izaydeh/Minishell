/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:51:24 by shoaib            #+#    #+#             */
/*   Updated: 2025/02/24 18:59:36 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_count_subwords(char *s)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == ' ')
			s++;
		if (*s)
		{
			count++;
			if (*s == '"')
				skip_quoted_word(&s);
			else
				while (*s && *s != ' ')
					s++;
		}
	}
	return (count);
}

char *extract_token_with_quotes(char **s)
{
    size_t	len;
	size_t	pos;
	char	*token;
	char	quote;

    len = strlen(*s);
    token = malloc(len + 1);
    if (!token) { perror("malloc"); exit(EXIT_FAILURE); }
    pos = 0;
    while (**s && !isspace((unsigned char)**s))
    {
        if (**s == '\'' || **s == '"')
        {
            quote = **s;
            (*s)++;
            while (**s && **s != quote)
            {
                token[pos++] = **s;
                (*s)++;
            }
            if (**s == quote) (*s)++;
        }
        else { token[pos++] = **s; (*s)++; }
    }
    token[pos] = '\0';
    return (token);
}

char **ft_split_whitespace(char *s)
{
    char **tokens;
    size_t count = 0;
    size_t i = 0;
    char *str = s;
    char *token;

    // First pass: Count the tokens.
    while (*str)
    {
        while (*str && isspace((unsigned char)*str))
            str++;
        if (*str)
        {
            count++;
            while (*str && !isspace((unsigned char)*str))
            {
                if (*str == '\'' || *str == '"')
                {
                    char quote = *str;
                    str++;  // Skip the opening quote.
                    while (*str && *str != quote)
                        str++;
                    if (*str == quote)
                        str++;  // Skip the closing quote.
                }
                else
                    str++;
            }
        }
    }
    tokens = malloc((count + 1) * sizeof(char *));
    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    while (*s)
    {
        while (*s && isspace((unsigned char)*s))
            s++;
        if (*s)
        {
            token = extract_token_with_quotes(&s);
            tokens[i++] = token;
        }
    }
    tokens[i] = NULL;
    return tokens;
}

char	**parse_echo_arguments(char *s, char **tokens)
{
	const char	*start;

	while (*s == ' ')
		s++;
	if (*s == '-')
	{
		start = s;
		while (*s && *s != ' ')
			s++;
		tokens[1] = strndup(start, s - start);
		while (*s == ' ')
			s++;
		tokens[2] = strdup(s);
	}
	else
	{
		tokens[1] = strdup(s);
		tokens[2] = NULL;
	}
	return (tokens);
}

char	**ft_split_echo(char *s)
{
	char		**tokens;
	const char	*start;

	tokens = malloc(4 * sizeof(char *));
	if (!tokens)
		return (NULL);
	while (*s == ' ')
		s++;
	start = s;
	while (*s && *s != ' ')
		s++;
	tokens[0] = strndup(start, s - start);
	tokens = parse_echo_arguments(s, tokens);
	tokens[3] = NULL;
	return (tokens);
}
