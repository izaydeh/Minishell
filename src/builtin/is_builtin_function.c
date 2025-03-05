/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin_function.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:35:06 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 19:35:07 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_funcion(t_shell *test, int i)
{
	if (strcmp(test->command[i][0], "cd") == 0 || strcmp(test->command[i][0],
			"pwd") == 0 || strcmp(test->command[i][0], "env") == 0 ||
		strcmp(test->command[i][0], "echo") == 0 || strcmp(test->command[i][0],
				"unset") == 0 ||
		strcmp(test->command[i][0], "export") == 0
			|| strcmp(test->command[i][0], "exit") == 0)
		return (1);
	return (0);
}

void	run_builtin_function(t_shell *shell, int i, int out_fd)
{
	int	stdout;

	stdout = dup(STDOUT_FILENO);
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (strcmp(shell->command[i][0], "unset") == 0)
		ft_unset(shell, shell->command[i]);
	else if (strcmp(shell->command[i][0], "export") == 0)
		ft_export(shell, shell->command[i]);
	else if (ft_strcmp(shell->command[i][0], "echo") == 0)
		ft_echo(shell, i);
	else if (ft_strcmp(shell->command[i][0], "cd") == 0)
		ft_cd(shell, i);
	else if (ft_strcmp(shell->command[i][0], "env") == 0)
		ft_env(shell, i);
	else if (ft_strcmp(shell->command[i][0], "pwd") == 0)
		ft_pwd(shell);
	dup2(stdout, STDOUT_FILENO);
	close(stdout);
}
