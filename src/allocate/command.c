/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:54:09 by shoaib            #+#    #+#             */
/*   Updated: 2025/03/01 05:11:12 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_max_commands(t_shell *test)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!test->input_splitted)
		return (0);
	while (test->input_splitted[i])
	{
		if (strcmp(test->input_splitted[i], "<<") != 0
			&& strcmp(test->input_splitted[i], "<") != 0
			&& strcmp(test->input_splitted[i], ">>") != 0
			&& strcmp(test->input_splitted[i], ">") != 0
			&& strcmp(test->input_splitted[i], "|") != 0 && (i == 0
				|| (strcmp(test->input_splitted[i - 1], "<<") != 0
					&& strcmp(test->input_splitted[i - 1], "<") != 0
					&& strcmp(test->input_splitted[i - 1], ">>") != 0
					&& strcmp(test->input_splitted[i - 1], ">") != 0)))
			{
				count++;
				test->command_count++;
			}
		i++;
	}
	return (count);
}

void	store_command(t_shell *test, int *j, int i)
{
	if (test->input_splitted[i] != NULL)
	{
		test->command[*j] = test->split_the_split[i];
		(*j)++;
	}
}

static int check_if(t_shell *test, int i)
{
	return (strcmp(test->input_splitted[i], "<<") != 0
			&& strcmp(test->input_splitted[i], "<") != 0
			&& strcmp(test->input_splitted[i], ">>") != 0
			&& strcmp(test->input_splitted[i], ">") != 0
			&& strcmp(test->input_splitted[i], "|") != 0 && (i == 0
				|| (strcmp(test->input_splitted[i - 1], "<<") != 0
					&& strcmp(test->input_splitted[i - 1], "<") != 0
					&& strcmp(test->input_splitted[i - 1], ">>") != 0
					&& strcmp(test->input_splitted[i - 1], ">") != 0)));
}
void	process_commands(t_shell *test)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	int pipe = 0;
	while (test->input_splitted[i])
	{
		if (strcmp(test->input_splitted[i], "|") == 0)
			pipe += 1;
		if (check_if(test, i))
		{
			store_command(test, &j, i);
			if (pipe > 0)
				pipe -= 1;
		}
		else if (pipe == 2 || (pipe == 1 && test->input_splitted[i + 1] == NULL))
		{
			test->command[j++] = NULL;
			pipe -= 1;
		}
		i++;
	}
	test->command[j] = NULL;
}

void	command_count(t_shell *test)
{
	int x;

	x = count_max_commands(test);
	if (x > test->count_pipe)
		test->command = malloc(sizeof(char **) * (x + 1));
	else
		test->command = malloc(sizeof(char **) * (test->count_pipe + 2));
	if (!test->command)
	{
		printf("Memory allocation failed for command array\n");
		return ;
	}
	process_commands(test);
}
