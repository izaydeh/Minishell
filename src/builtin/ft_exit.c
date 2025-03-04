/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:47:55 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/03/02 07:17:38 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h> 

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] && str[i] != ' ')
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_argc_in_exit(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] == 32)
			return (0);
		i++;
	}
	return (1);
}

int	validate_exit_arguments(t_shell *shell, int i)
{
	long long	val;

	if (!shell->command[i])
	{
		shell->exit_status = 0;
		return (1);
	}
	if (!is_numeric(shell->command[i][1]))
	{
		print_error(shell->name_program, shell->command[i][1],
				"numeric argument required");
		shell->exit_status = 2;
		return (1);
	}
	val = ft_atoi(shell->command[i][1]);
	if (val == LLONG_MAX || val == LLONG_MIN)
	{
		print_error(shell->name_program, shell->command[i][1], 
			"numeric argument required");
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

int	ft_exit(t_shell *shell, int i)
{
	long	exit_int;

	printf("exit\n");
	if (validate_exit_arguments(shell, i))
		return (1);
	if (shell->command[i][2])
	{
		print_error(shell->name_program, "", "too many arguments");
		shell->exit_status = 1;
		return (0);
	}
	exit_int = ft_atoi(shell->command[i][1]);
	if (exit_int > 255)
	{
		shell->exit_status = exit_int % 256;
		return (1);
	}
	if (exit_int < 0)
	{
		shell->exit_status = (exit_int % 256 + 256) % 256;
		return (1);
	}
	shell->exit_status = exit_int;
	return (1);
}
