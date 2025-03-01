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

void	handle_echo_flag(t_shell *string, int i, int *x, int *f)
{
	while (string->command[i][1][*x])
	{
		if (string->command[i][1][*x] != 'n'
			&& string->command[i][1][*x] != 'e')
		{
			*f = 1;
			break ;
		}
		(*x)++;
	}
}

void	print_echo_arguments(t_shell *string, int i, int x)
{
	int	j;

	while (string->command[i][x])
	{
		j = 0;
		while (string->command[i][x][j])
		{
			write(1, &string->command[i][x][j], 1);
			j++;
		}
		if (string->command[i][x + 1])
			write(1, " ", 1);
		x++;
	}
}

void	ft_echo(t_shell *string, int i)
{
	int	x;
	int	f;

	x = 1;
	f = 0;
	if (!string->command[i][1])
	{
		write(1, "\n", 1);
		return ;
	}
	if (string->command[i][1][0] == '-'
		&& string->command[i][1][1] == 'n')
	{
		handle_echo_flag(string, i, &x, &f);
		if (f == 0)
		{
			x = 2;
			print_echo_arguments(string, i, x);
			return ;
		}
	}
	x = 1;
	print_echo_arguments(string, i, x);
	write(1, "\n", 1);
}
