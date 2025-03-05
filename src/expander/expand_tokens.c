/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 02:03:58 by marvin            #+#    #+#             */
/*   Updated: 2025/03/02 03:20:35 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_character(char **s, t_shell *shell, t_exp *exp)
{
	if (**s == '\\')
	{
		(*s)++;  // Skip the backslash
		// If the next char is one of the special ones, output it normally.
		if (**s == '\"' || **s == '\\' || **s == '$' || **s == '`')
		{
			*(exp->out) = **s;
			exp->out++;
			(*s)++;
		}
		else
		{
			// NEW: For any other character, skip the backslash and output the character alone.
			if (**s)
			{
				*(exp->out) = **s;
				exp->out++;
				(*s)++;
			}
		}
	}
	else if (**s == '$')
		*s = process_dollar(*s, shell, exp);
	else
	{
		*(exp->out) = **s;
		exp->out++;
		(*s)++;
	}
	return (*s);
}

char	*handle_double_quote(char *s, t_shell *shell, t_exp *exp)
{
	s++;
	while (*s && *s != '\"')
	{
		s = process_character(&s, shell, exp);
	}
	if (*s == '\"')
		s++;
	return (s);
}

char	*process_token(char *s, t_shell *shell, t_exp *exp)
{
	while (*s)
	{
		if (*s == '\'')
			s = handle_signal_quote(s, exp);
		else if (*s == '\"')
			s = handle_double_quote(s, shell, exp);
		else
			s = handle_un_quotetd(s, shell, exp);
	}
	return (s);
}

char	*expander(char *token, t_shell *shell)
{
	t_exp	exp;
	char	*s;
	char	*final;
	char	*original;

	original = token;
	exp.res = malloc(8192);
	if (exp.res == NULL)
		return (NULL);
	exp.out = exp.res;
	s = token;
	s = process_token(s, shell, &exp);
	*exp.out = '\0';
	if (original[0] == '$')
	{
		final = d_delete_spaces(exp.res);
		free(exp.res);
		return (final);
	}
	else if (should_delete_sp(original))
	{
		final = delete_spaces(exp.res);
		free(exp.res);
		return (final);
	}
	return (exp.res);
}
