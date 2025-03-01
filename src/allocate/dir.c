/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:54:13 by shoaib            #+#    #+#             */
/*   Updated: 2025/03/01 04:23:03 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_max_dir(t_shell *test, int y)
{
	int	count;
	int	i;
	int	command_index;

	count = 0;
	i = 0;
	command_index = 0;
	if (!test->input_splitted)
		return (0);
	while (test->input_splitted[i])
	{
		if (strcmp(test->input_splitted[i], "|") == 0)
			command_index++;
		else if (command_index == y)
		{
			if (strcmp(test->input_splitted[i], "<") == 0
				|| strcmp(test->input_splitted[i], ">") == 0
				|| strcmp(test->input_splitted[i], "<<") == 0
				|| strcmp(test->input_splitted[i], ">>") == 0)
				count++;
		}
		i++;
	}
	return (count);
}

void	store_redirect(t_shell *test, int *j, int y, int *x)
{
	if (test->input_splitted[*j + 1])
	{
		test->dir[y][*x] = test->input_splitted[*j + 1];
		test->dir_count++;
		(*x)++;
		(*j)++;
	}
}

void	process_dir_input(t_shell *test, int *j, int y)
{
	int	x;

	x = 0;
	while (test->input_splitted[*j] && (strcmp(test->input_splitted[*j],
				"|") != 0))
	{
		if (strcmp(test->input_splitted[*j], "<") == 0
			|| strcmp(test->input_splitted[*j], ">") == 0
			|| strcmp(test->input_splitted[*j], "<<") == 0
			|| strcmp(test->input_splitted[*j], ">>") == 0)
		{
			store_redirect(test, j, y, &x);
		}
		(*j)++;
	}
	test->dir[y][x] = NULL;
	if (test->input_splitted[*j])
		(*j)++;
}

void	dir(t_shell *test)
{
	int	j;
	int	y;

	j = 0;
	y = 0;
	test->dir_count = 0;
	test->dir = malloc(sizeof(char **) * (test->count_pipe + 2));
	if (!test->dir)
		return ;
	while (y < test->count_pipe + 1)
	{
		test->dir[y] = malloc(sizeof(char *) * (count_max_dir(test, y) + 1));
		if (!test->dir[y])
			return ;
		process_dir_input(test, &j, y);
		y++;
	}
	test->dir[y] = NULL;
}
