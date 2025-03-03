/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:29:43 by shoaib            #+#    #+#             */
/*   Updated: 2025/03/02 05:57:35 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2d(char **ar)
{
	int	i;

	i = 0;
	if (!ar)
		return ;
	while ((ar)[i])
	{
		free((ar)[i]);
		i++;
	}
	free(ar);
	ar = NULL;
}

void	free_3d(char ***ar)
{
	int	i;

	i = 0;
	if (!ar || !*ar)
		return ;
	while ((ar)[i])
	{
		free_2d(((ar)[i]));
		i++;
	}
	free(ar);
	ar = NULL;
}

void	free_child_resources(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		if (shell->input_splitted)
		{
			free_2d(shell->input_splitted);
			// free(shell->input_splitted);
			free(shell->split_the_split);
			free(shell->command);
			// free_3d(shell->dir);
			// free_2d(shell->dir);
			int i = 0;
			while (shell->dir && shell->dir[i])
				free(shell->dir[i++]);
			if (shell->dir)
				free(shell->dir);
			// free_3d(shell->operate);
			i = 0;
			while (shell->operate && shell->operate[i])
				free(shell->operate[i++]);
			if (shell->operate)
				free(shell->operate);
			shell->input = NULL;
			shell->input_splitted = NULL;
			shell->split_the_split = NULL;
			shell->command = NULL;
			shell->dir = NULL;
			shell->operate = NULL;
			shell->command_count = 0;
			shell->dir_count = 0;
			shell->operate_count = 0;
			shell->count_pipe = 0;
		}
	}
}

void	free_shell(t_shell *shell, int f, int child)
{
	if (child)
		free_child_resources(shell);
	if (f)
	{
		free_2d(shell->env);
		shell->env = NULL;
		if (shell->old_pwd)
		{
			free(shell->old_pwd);
			shell->old_pwd = NULL;
		}
	}
}

void	e_exit(t_shell *shell, int i)
{
	shell->exit_status = i;
	exit(i);
}
