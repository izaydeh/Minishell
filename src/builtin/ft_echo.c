/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:34:53 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/03/01 04:58:49 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_echo_arguments(t_shell *string, int i, int start)
{
	int	j;

	while (string->command[i][start])
	{
		j = 0;
		while (string->command[i][start][j])
		{
			write(1, &string->command[i][start][j], 1);
			j++;
		}
		if (string->command[i][start + 1])
			write(1, " ", 1);
		start++;
	}
}
void	ft_echo(t_shell *string, int i)
{
	int	arg_index;
	int	flag_no_newline;
	int	j;
	int	valid;
	char *home;

	arg_index = 1;
	flag_no_newline = 0;
	if (strcmp(string->command[i][1], "~") == 0 && string->command[i][2] == NULL)
	{
		home = get_env_value(string, "HOME");
		printf("%s\n", home);
		return ;
	}
	if (!string->command[i][1])
	{
		write(1, "\n", 1);
		return ;
	}
	while (string->command[i][arg_index] && 
	       string->command[i][arg_index][0] == '-' &&
	       string->command[i][arg_index][1])
	{
		j = 1;
		valid = 1;
		while (string->command[i][arg_index][j])
		{
			if (string->command[i][arg_index][j] != 'n' &&
			    string->command[i][arg_index][j] != 'e')
			{
				valid = 0;
				break;
			}
			j++;
		}
		if (valid)
		{
			flag_no_newline = 1;
			arg_index++;
		}
		else
			break;
	}
	print_echo_arguments(string, i, arg_index);
	if (!flag_no_newline)
		write(1, "\n", 1);
}
