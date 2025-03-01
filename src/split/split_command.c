/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:17:57 by shoaib            #+#    #+#             */
/*   Updated: 2025/02/24 18:58:16 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize_input(t_shell *g_shell, const char *input, char **tokens,
		int max_tokens, int *cmd_flags)
{
	int			token_count;
	int			last_cmd_index;
	int			has_command;
	const char	*p;
	int			prev_redir;
	int			op_len;
	char		*op_token;
	const char	*start_ptr;
	int			in_quote;
	char		quote_char;
	int			part_len;
	char		*part;
	int			trimmed_len;
	char		*space;
	int			word_len;
	char		*operand;
	char		*expanded_operand;
	char		*rem;
	int			extra_len;
	size_t		old_len;
	char		*merged;
	char		*extra;
	char		*expanded_extra;
	char		*token_str;
	char		*trim_start;
	char		*trim_end;

	token_count = 0;
	last_cmd_index = -1;
	has_command = 0;
	p = input;
	prev_redir = 0;
	while (*p != '\0' && token_count < max_tokens)
	{
		while (*p && isspace((unsigned char)*p))
			p++;
		if (*p == '\0')
			break ;
		op_len = 0;
		if (is_operator(p, &op_len))
		{
			op_token = malloc(op_len + 1);
			if (!op_token)
			{
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			strncpy(op_token, p, op_len);
			op_token[op_len] = '\0';
			tokens[token_count] = op_token;
			cmd_flags[token_count] = -1;
			token_count++;
			if (strcmp(op_token, "|") == 0)
			{
				has_command = 0;
				last_cmd_index = -1;
			}
			if (is_redirection_operator(op_token))
				prev_redir = 1;
			else
				prev_redir = 0;
			p += op_len;
		}
		else
		{
			start_ptr = p;
			in_quote = 0;
			quote_char = '\0';
			while (*p != '\0')
			{
				if (*p == '\'' || *p == '"')
				{
					if (!in_quote)
					{
						in_quote = 1;
						quote_char = *p;
					}
					else if (quote_char == *p)
					{
						in_quote = 0;
						quote_char = '\0';
					}
					p++;
					continue ;
				}
				if (!in_quote && is_operator(p, &op_len))
					break ;
				p++;
			}
			part_len = p - start_ptr;
			part = malloc(part_len + 1);
			if (!part)
			{
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			strncpy(part, start_ptr, part_len);
			part[part_len] = '\0';
			trim(part, &trim_start, &trim_end);
			trimmed_len = trim_end - trim_start;
			if (trimmed_len <= 0)
			{
				free(part);
				prev_redir = 0;
				continue ;
			}
			if (prev_redir)
			{
				space = NULL;
				for (int i = 0; i < trimmed_len; i++)
				{
					if (isspace((unsigned char)trim_start[i]))
					{
						space = trim_start + i;
						break ;
					}
				}
				if (space)
				{
					word_len = space - trim_start;
					operand = malloc(word_len + 1);
					if (!operand)
					{
						perror("malloc");
						exit(EXIT_FAILURE);
					}
					strncpy(operand, trim_start, word_len);
					operand[word_len] = '\0';
					expanded_operand = expander(operand, g_shell);
					free(operand);
					tokens[token_count++] = expanded_operand;
					cmd_flags[token_count - 1] = 0;
					rem = space;
					while (rem < trim_end && isspace((unsigned char)*rem))
						rem++;
					if (rem < trim_end)
					{
						extra_len = trim_end - rem;
						if (has_command && last_cmd_index != -1
							&& cmd_flags[last_cmd_index] == 1)
						{
							old_len = strlen(tokens[last_cmd_index]);
							merged = malloc(old_len + 1 + extra_len + 1);
							if (!merged)
							{
								perror("malloc");
								exit(EXIT_FAILURE);
							}
							sprintf(merged, "%s %.*s", tokens[last_cmd_index],
								extra_len, rem);
							free(tokens[last_cmd_index]);
							tokens[last_cmd_index] = merged;
						}
						else
						{
							extra = malloc(extra_len + 1);
							if (!extra)
							{
								perror("malloc");
								exit(EXIT_FAILURE);
							}
							strncpy(extra, rem, extra_len);
							extra[extra_len] = '\0';
							expanded_extra = expander(extra, g_shell);
							free(extra);
							tokens[token_count++] = expanded_extra;
							cmd_flags[token_count - 1] = 0;
							has_command = 1;
							last_cmd_index = token_count - 1;
						}
					}
				}
				else
				{
					operand = malloc(trimmed_len + 1);
					if (!operand)
					{
						perror("malloc");
						exit(EXIT_FAILURE);
					}
					strncpy(operand, trim_start, trimmed_len);
					operand[trimmed_len] = '\0';
					expanded_operand = expander(operand, g_shell);
					free(operand);
					tokens[token_count++] = expanded_operand;
					cmd_flags[token_count - 1] = 0;
				}
				prev_redir = 0;
			}
			else
			{
				token_str = malloc(trimmed_len + 1);
				if (!token_str)
				{
					perror("malloc");
					exit(EXIT_FAILURE);
				}
				strncpy(token_str, trim_start, trimmed_len);
				token_str[trimmed_len] = '\0';
				tokens[token_count++] = token_str;
				cmd_flags[token_count - 1] = 1;
				has_command = 1;
				last_cmd_index = token_count - 1;
			}
			free(part);
		}
	}
	return (token_count);
}
