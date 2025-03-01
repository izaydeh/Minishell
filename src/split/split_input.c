/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:54:58 by shoaib            #+#    #+#             */
/*   Updated: 2025/02/24 19:06:29 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split(const char *input, t_shell *shell)
{
	const int	max_tokens = 100;
	char		**tokens;
	int			*cmd_flags;
	int			token_count;

	tokens = malloc((max_tokens + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd_flags = malloc(max_tokens * sizeof(int));
	if (!cmd_flags)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token_count = tokenize_input(shell, input, tokens, max_tokens, cmd_flags);
	tokens[token_count] = NULL;
	free(cmd_flags);
	return (tokens);
}

void	skip_quoted_word(char **s)
{
	(*s)++;
	while (**s && **s != '"')
		(*s)++;
	if (**s == '"')
		(*s)++;
}

char	***allocate_split(char **s, size_t *count)
{
	char	***final_split;

	*count = 0;
	if (!s)
		return (NULL);
	while (s[*count])
		(*count)++;
	final_split = malloc((*count + 1) * sizeof(char **));
	if (!final_split)
		return (NULL);
	final_split[*count] = NULL;
	return (final_split);
}

char	***split_commands(char **s)
{
	char	***final_split;
	size_t	count;
	size_t	i;

	final_split = allocate_split(s, &count);
	if (!final_split)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (!ft_strncmp(s[i], "echo", 4) && (s[i][4] == ' ' || s[i][4] == '\0'))
			final_split[i] = ft_split_echo(s[i]);
		else
			final_split[i] = ft_split_whitespace(s[i]);
		i++;
	}
	return (final_split);
}
