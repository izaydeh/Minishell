/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:04:34 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 20:05:12 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_extra_spaces(char *s, char *new)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (s[i] == '\0')
			break ;
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

char	*delete_spaces(char *s)
{
	int		len;
	char	*new;

	len = ft_strlen(s);
	new = malloc(len + 1);
	if (new == NULL)
		return (NULL);
	return (remove_extra_spaces(s, new));
}

int	should_delete_sp(char *token)
{
	int		start;
	int		end;
	int		len;
	char	c;

	if (token == NULL || token[0] == '\0')
		return (1);
	c = token[0];
	if (c != '\"' && c != '\'')
		return (1);
	start = 0;
	while (token[start] && token[start] == c)
		start++;
	len = ft_strlen(token);
	end = len - 1;
	while (end >= 0 && token[end] == c)
		end--;
	if ((start % 2) == 1 && (((len - 1) - end) % 2) == 1)
		return (0);
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
		s++;
	return (s);
}

char	*handle_un_quotetd(char *s, t_shell *shell, t_exp *exp)
{
	while (*s && *s != '\"' && *s != '\'')
	{
		if (*s == '$')
			s = process_dollar(s, shell, exp);
		else
		{
			*(exp->out) = *s;
			exp->out++;
			s++;
		}
	}
	return (s);
}
