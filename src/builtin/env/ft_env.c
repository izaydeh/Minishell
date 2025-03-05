/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:32:59 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 19:33:00 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_shell *test, int i)
{
	int	x;
	char **argv;

	argv = test->command[i];
	if (argv[1] != NULL)
	{
		if (argv[1][strlen(argv[1]) - 1] == '/')
		{
			argv[1][strlen(argv[1]) - 1] = '\0';
			int dir_check = check_dir(argv[1]);
			if (dir_check == 1 || dir_check == 3)
			{
				print_error(test->name_program, argv[1], "Permission denied");
				test->exit_status = 126;
			}
			else if (dir_check == 2)
			{
				print_error(test->name_program, argv[1], "Not a directory");
				test->exit_status = 126;
			}
			else
			{
				print_error(test->name_program, argv[1], "No such file or directory");
				test->exit_status = 127;
			}
		}
		else
		{
			printf("env: ‘%s’: No such file or directory\n", test->command[i][1]);
			test->exit_status = 127;
		}
		return ;
	}
	x = 0;
	while (test->env[x])
	{
		write(1, test->env[x], ft_strlen(test->env[x]));
		write(1, "\n", 1);
		x++;
	}
	test->exit_status = 0;
}

char	*get_env_value(t_shell *shell, char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (shell->env[i])
	{
		if (strncmp(shell->env[i], key, len) == 0 && shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	ft_env_copy(t_shell *test, char **environ, int i)
{
	int	j;

	while (environ[i])
	{
		test->env[i] = malloc((strlen(environ[i]) + 1) * sizeof(char));
		if (!test->env[i])
			return ;
		j = 0;
		while (environ[i][j])
		{
			test->env[i][j] = environ[i][j];
			j++;
		}
		test->env[i][j] = '\0';
		i++;
	}
	test->env[i] = NULL;
}

void	ft_env_init(t_shell *test)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
		i++;
	test->env = malloc((i + 1) * sizeof(char *));
	if (!test->env)
		return ;
	ft_env_copy(test, environ, 0);
	test->env[i] = NULL;
	test->exp = NULL;
}
