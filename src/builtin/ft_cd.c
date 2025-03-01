/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:34:47 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 19:34:48 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_directory(t_shell *shell, int i, char **target)
{
	char	*home;

	home = get_env_value(shell, "HOME");
	if (!shell->command[i][1] || strcmp(shell->command[i][1], "~") == 0
		|| strcmp(shell->command[i][1], "--") == 0)
	{
		if (!home)
		{
			perror("cd: HOME not set\n");
			shell->exit_status = 1;
			*target = NULL;
			return ;
		}
		*target = home;
	}
	else
	{
		*target = shell->command[i][1];
		if (check_dir(*target) != 1)
		{
			write(2, "the argument not a directory \n", 31);
			*target = NULL;
		}
	}
}

void	ft_cd(t_shell *shell, int i)
{
	char	*target;

	target = NULL;
	get_directory(shell, i, &target);
	if (!target)
		return ;
	get_old_pwd(shell);
	if (chdir(target) != 0)
	{
		perror("cd");
		shell->exit_status = 1;
	}
	else
	{
		update_pwd(shell);
		shell->exit_status = 0;
	}
}
