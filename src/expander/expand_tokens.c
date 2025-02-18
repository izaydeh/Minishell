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

char	*delete_spaces(char *s)
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
			break ;
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

int	should_delete_sp(char *token)
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

char	*handle_signal_quote(char *s, t_exp *exp)
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

char	*handle_un_quotetd(char *s, t_shell *shell, t_exp *exp)
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

char	*handle_double_quote(char *s, t_shell *shell, t_exp *exp)
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

char	*expander(char *token, t_shell *shell)
{
	t_exp		exp;
	char		*s;
	size_t		len;
	char		*final;
    char        *original;

	original = token; 
	len = ft_strlen(token);
	exp.res = malloc(8192);
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
			s = handle_signal_quote(s, &exp);
		}
		else if (*s == '\"')
		{
			s = handle_double_quote(s, shell, &exp);
		}
		else
		{
			s = handle_un_quotetd(s, shell, &exp);
		}
	}
	*exp.out = '\0';
	// Use the original token (or a copy) for determining collapse.
	if (should_delete_sp(original))
	{
		final = delete_spaces(exp.res);
		free(exp.res);
		return (final);
	}
	return (exp.res);
}
