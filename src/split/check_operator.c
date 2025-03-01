/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:50:02 by shoaib            #+#    #+#             */
/*   Updated: 2025/02/26 20:21:28 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_operator(const char *op)
{
	if (op == NULL)
	 return 0;
	return (strcmp(op, ">") == 0 || strcmp(op, ">>") == 0
		|| strcmp(op, "<") == 0 || strcmp(op, "<<") == 0);
}

void	trim(char *str, char **start, char **end)
{
	char	*last;

	while (isspace((unsigned char)*str))
		str++;
	*start = str;
	if (*str == '\0')
	{
		*end = str;
		return ;
	}
	last = str + strlen(str) - 1;
	while (last >= str && isspace((unsigned char)*last))
		last--;
	*end = last + 1;
}

int	is_operator(const char *s, int *op_len)
{
	if (s[0] == '>' && s[1] == '>')
	{
		*op_len = 2;
		return (1);
	}
	else if (s[0] == '<' && s[1] == '<')
	{
		*op_len = 2;
		return (1);
	}
	else if (s[0] == '<' || s[0] == '>' || s[0] == '|')
	{
		*op_len = 1;
		return (1);
	}
	*op_len = 0;
	return (0);
}
