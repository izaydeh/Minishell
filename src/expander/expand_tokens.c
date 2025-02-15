/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 02:03:58 by marvin            #+#    #+#             */
/*   Updated: 2025/02/11 02:03:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collapse_whitespace(const char *s)
{
	int		i;
	int		j;
	int		len;
	char	*new;

	len = ft_strlen(s);
	new = malloc(len + 1);
	if (new == NULL)
	{
		return (NULL);
	}
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == ' ')
		{
			i++;
		}
		if (s[i] == '\0')
		{
			break;
		}
		if (j > 0)
		{
			new[j] = ' ';
			j++;
		}
		while (s[i] && s[i] != ' ')
		{
			new[j] = s[i];
			j++;
			i++;
		}
	}
	new[j] = '\0';
	return (new);
}

static int	should_collapse(const char *token)
{
	int		start;
	int		end;
	int		len;
	char	c;

	if (token == NULL || token[0] == '\0')
	{
		return (1);
	}
	c = token[0];
	if (c != '\"' && c != '\'')
	{
		return (1);
	}
	start = 0;
	while (token[start] && token[start] == c)
	{
		start++;
	}
	len = ft_strlen(token);
	end = len - 1;
	while (end >= 0 && token[end] == c)
	{
		end--;
	}
	if ((start % 2) == 1 && (((len - 1) - end) % 2) == 1)
	{
		return (0);
	}
	return (1);
}

static const char	*process_dollar(const char *s, t_shell *shell, t_exp *exp)
{
	char	var[128];
	int		i;
	char	*value;
	int		j;

	s++;
	if (*s == '\0' || (ft_isalpha(*s) == 0 && *s != '_'))
	{
		*(exp->out) = '$';
		exp->out++;
		return (s);
	}
	i = 0;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_') && i < 127)
	{
		var[i] = s[i];
		i++;
	}
	var[i] = '\0';
	s = s + i;
	value = get_env_value(shell, var);
	j = 0;
	while (value[j])
	{
		*(exp->out) = value[j];
		exp->out++;
		j++;
	}
	return (s);
}

static const char	*process_single(const char *s, t_exp *exp)
{
	s++;
	while (*s && *s != '\'')
	{
		*(exp->out) = *s;
		exp->out++;
		s++;
	}
	if (*s == '\'')
	{
		s++;
	}
	return (s);
}

static const char	*process_double(const char *s, t_shell *shell, t_exp *exp)
{
	s++;
	while (*s && *s != '\"')
	{
		if (*s == '\\')
		{
			s++;
			if (*s == '\"' || *s == '\\' || *s == '$' || *s == '`')
			{
				*(exp->out) = *s;
				exp->out++;
				s++;
			}
			else
			{
				*(exp->out) = '\\';
				exp->out++;
			}
		}
		else if (*s == '$')
		{
			s = process_dollar(s, shell, exp);
		}
		else
		{
			*(exp->out) = *s;
			exp->out++;
			s++;
		}
	}
	if (*s == '\"')
	{
		s++;
	}
	return (s);
}

static const char	*process_unquoted(const char *s, t_shell *shell, t_exp *exp)
{
	while (*s && *s != '\"' && *s != '\'')
	{
		if (*s == '$')
		{
			s = process_dollar(s, shell, exp);
		}
		else
		{
			*(exp->out) = *s;
			exp->out++;
			s++;
		}
	}
	return (s);
}

char	*expand_token(const char *token, t_shell *shell)
{
	t_exp		exp;
	const char	*s;
	size_t		len;
	char		*final;

	len = ft_strlen(token);
	exp.res = malloc(len * 2 + 1);
	if (exp.res == NULL)
	{
		return (NULL);
	}
	exp.out = exp.res;
	s = token;
	while (*s)
	{
		if (*s == '\'')
		{
			s = process_single(s, &exp);
		}
		else if (*s == '\"')
		{
			s = process_double(s, shell, &exp);
		}
		else
		{
			s = process_unquoted(s, shell, &exp);
		}
	}
	*exp.out = '\0';
	if (should_collapse(token))
	{
		final = collapse_whitespace(exp.res);
		free(exp.res);
		return (final);
	}
	return (exp.res);
}
