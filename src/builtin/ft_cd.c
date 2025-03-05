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
			printf("%s: cd: HOME not set\n", shell->name_program);
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
			if (check_dir(*target) == 3)
				printf("%s: cd: Permission Denied\n", shell->name_program);
			shell->exit_status = 1;
			*target = NULL;
		}
	}
}

void	ft_cd(t_shell *shell, int i)
{
	char	*target;
	char	*cwd_after;
	
	target = NULL;
	if (shell->command[i][2] != NULL && shell->command[i][1] != NULL)
	{
		printf("%s: cd: too many arguments\n", shell->name_program);
		shell->exit_status = 1;
		return ;
	}
	get_directory(shell, i, &target);
	if (!target && strcmp(shell->command[i][1], "..") != 0)
	{
		printf("%s: cd: no such file or directory\n", shell->name_program);
		shell->exit_status = 1;
		return ;
	}
	cwd_after = getcwd(NULL, 0);
	get_old_pwd(shell);
	if (chdir(target) != 0)
	{
		if (!cwd_after)
		{
			printf("%s: error retrieving current directory:", shell->name_program);
			printf(" getcwd: cannot access parent directories: No such file or directory\n");
			shell->exit_status = 0;
		}
		else
		{
			perror("cd");
			shell->exit_status = 1;
		}
		return ;
	}
	else
	{
		update_pwd(shell);
		shell->exit_status = 0;
		free(cwd_after);
	}
}
