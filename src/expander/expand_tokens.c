/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 02:03:58 by marvin            #+#    #+#             */
/*   Updated: 2025/02/25 20:05:15 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_character(char **s, t_shell *shell, t_exp *exp)
{
	if (**s == '\\')
	{
		(*s)++;
		if (**s == '\"' || **s == '\\' || **s == '$' || **s == '`')
		{
			*(exp->out) = **s;
			exp->out++;
			(*s)++;
		}
		else
		{
			*(exp->out) = '\\';
			exp->out++;
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
	if (should_delete_sp(original))
	{
		final = delete_spaces(exp.res);
		free(exp.res);
		return (final);
	}
	return (exp.res);
}
