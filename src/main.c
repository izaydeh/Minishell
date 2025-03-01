/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/03/01 06:20:30 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void run_command(t_shell *test)
{
		int h = test->operate_count + test->command_count;
		if (test->count_pipe >= 1 && ((test->count_pipe + 1) != test->command_count)
			&& ((test->count_pipe + 1) != test->operate_count) && ((test->count_pipe + 1) != h))
		{
			ft_printf("%s: syntax error near unexpected token `newline'\n", test->name_program);
			test->exit_status = 2;
		}
		else if (test->command_count == 0 && test->count_pipe == 0
			&& test->dir_count == 0)
		{
			if (msg_operate_error(test))
				test->exit_status = 2;
		}
		else if (test->command_count == 1 && test->count_pipe == 0 &&
					test->input_splitted[0][strlen(test->input_splitted[0])
						- 1] == '/' && strcmp(test->command[0][0], "cd") != 0)
		{
			if (check_dir(test->input_splitted[0]) == 1)
				printf("%s is a directory.\n", test->input_splitted[0]);
		}
		else if (test->command && test->command[0] &&
					strcmp(test->command[0][0], "cd") == 0
						&& test->command_count == 1)
			ft_cd(test, 0);
		else if (test->command && test->command[0] &&
					strcmp(test->command[0][0], "export") == 0)
			ft_export(test, test->command[0]);
		else if (test->command_count == 1 && test->count_pipe == 0 
					&& test->command && test->command[0] &&
					strcmp(test->command[0][0], "exit") == 0)
		{
			if (ft_exit(test, 0))
				exit(test->exit_status);
		}
		else if (test->command && test->command[0] &&
					strcmp(test->command[0][0], "unset") == 0)
			ft_unset(test, test->command[0]);
		else
			execute_pipeline(test);
}

int	main(int argc, char **argv)
{
	t_shell test;
	int i;
	int j;
	char *expanded;

	(void)argc;
	// ft_bzero(&test, sizeof(t_shell));
	test.input = NULL;
	test.input_splitted = NULL;
	test.split_the_split = NULL;
	test.command = NULL;
	test.dir = NULL;
	test.operate = NULL;
	test.env = NULL;
	test.old_pwd = NULL;
	test.name_program = NULL;
	test.command_count = 0;
	test.exit_status = 0;
	test.operate_count = 0;
	test.dir_count = 0;
	test.count_pipe = 0;
	test.flag_here = 0;
	ft_env_init(&test);
	re_shlvl(&test);
	test.name_program = argv[0];
	while (1)
	{
		g_shell_signal = 0;
		signal(SIGINT, handle_signals); // Handle Ctrl+C => New line
		signal(SIGQUIT, SIG_IGN);       // Ignore Ctrl+ /
		test.input = readline("welcome to (shell)> ");
		if (!test.input)
		{
			write(2, "exit\n", 5);
			break ;
		}
		if (g_shell_signal == SIGINT)
			test.exit_status = 130;
		if (!test.input[0])
		{
			free_shell(&test, 0, 1);
			continue ;
		}
		test.input_splitted = ft_split(test.input, &test);
		test.split_the_split = split_commands(test.input_splitted);
		i = 0;
		while (test.split_the_split[i])
		{
			j = 0;
			while (test.split_the_split[i][j])
			{
				expanded = expander(test.split_the_split[i][j], &test);
				free(test.split_the_split[i][j]);
				test.split_the_split[i][j] = expanded;
				j++;
			}
			i++;
		}
		count_pipe(&test);
		command_count(&test);
		operate(&test);
		dir(&test);
		i = 0;
		int is_redirection_operatora(char op)
		{
		    return (op == '>' || op == '<');
		}
		if (test.dir_count >= 1)
		{
			while (test.input[i])
			{	
				if (is_redirection_operatora(test.input[i]))
				{
					if (test.input[i] == '<' || test.input[i] == '>')
					{
						if (test.input[i + 1] == test.input[i])
						{
							if (test.input[i + 2] == test.input[i] || 
								(test.input[i + 1] == '>' && test.input[i] == '<') || 
								(test.input[i + 1] == '<' && test.input[i] == '>'))
							{
								printf("%s: syntax error near unexpected token '%c%c'\n", test.name_program, test.input[i], test.input[i + 1]);
								test.exit_status = 2;
								break;
							}
						}
						else if ((test.input[i + 1] == '>' && test.input[i] == '<') || 
								 (test.input[i + 1] == '<' && test.input[i] == '>'))
						{
							printf("%s: syntax error near unexpected token '%c%c'\n", test.name_program, test.input[i], test.input[i + 1]);
							test.exit_status = 2;
							break;
						}
					}
				}
				i++;
			}
			if (test.exit_status == 2)
				continue ;
		}
		run_command(&test);
		add_history(test.input);
		free_shell(&test, 0, 1);
	}
	rl_clear_history();
	free_shell(&test, 1, 1);
	e_exit(&test, 0);
	return (0);
}
