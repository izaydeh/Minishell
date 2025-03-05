/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/03/02 23:51:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void run_command(t_shell *test)
{
		if ((!test->command || !test->command[0] || !test->command[0][0])
			&& test->operate_count == 0 && test->count_pipe == 0
				&& test->dir_count == 0)
			return;
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
		signal(SIGQUIT, handle_signals);       // Ignore Ctrl+ /
		test.input = readline("welcome to (shell)> ");
		if (g_shell_signal == SIGQUIT)
		{
			test.exit_status = 0;
			g_shell_signal = 0;
			continue;
		}
		if (!test.input)
		{
			write(2, "exit\n", 5);
			break ;
		}
		if (!test.input[0])
		{
			free_shell(&test, 0, 1);
			continue ;
		}
		add_history(test.input);
		if (g_shell_signal == SIGINT)
			test.exit_status = 130;
		test.input_splitted = ft_split(test.input, &test);
		test.split_the_split = split_commands(test.input_splitted);
		i = 0;
		while (test.split_the_split[i])
		{
			if (ft_strcmp(test.split_the_split[i][0], "export") == 0)
			{
				i++;
				continue;
			}
			j = 0;
			while (test.split_the_split[i][j])
			{
				if (test.split_the_split[i][j][0] == '$')
				{
					char ***split_the_split_$ = NULL;
					char **input_splitted_$ = NULL;
					expanded = expander(test.split_the_split[i][j], &test);
					if (expanded[0] == '\0')
					{
						free(expanded);
						free(test.split_the_split[i][j]);
						int k = j;
						while (test.split_the_split[i][k + 1] != NULL)
						{
							test.split_the_split[i][k] = test.split_the_split[i][k + 1];
							k++;
						}
						test.split_the_split[i][k] = NULL;
						continue;
					}					
					free(test.split_the_split[i][j]);
					input_splitted_$ = ft_split(expanded, &test);
					// free(expanded);
					split_the_split_$ = split_commands(input_splitted_$);
					// free(input_splitted_$);
					if (!split_the_split_$ || !split_the_split_$[0])
					{
						test.split_the_split[i][j] = strdup("");
						j++;
						test.split_the_split[i][j] = NULL;
						continue;
					}
					
					int f = 0;
					while (split_the_split_$[0][f])
					{
						test.split_the_split[i][j] = split_the_split_$[0][f];
						f++;
						j++;
					}
					test.split_the_split[i][j] = NULL;
					// free(split_the_split_$);
				}
				else
				{
					expanded = expander(test.split_the_split[i][j], &test);
					free(test.split_the_split[i][j]);
					test.split_the_split[i][j] = expanded;
					j++;
				}
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
								test.exit_status = 22;
								break;
							}
						}
						else if ((test.input[i + 1] == '>' && test.input[i] == '<') || 
								 (test.input[i + 1] == '<' && test.input[i] == '>'))
						{
							printf("%s: syntax error near unexpected token '%c%c'\n", test.name_program, test.input[i], test.input[i + 1]);
							test.exit_status = 22;
							break;
						}
					}
				}
				i++;
			}
			if (test.exit_status == 22)
			{
				test.exit_status = 2;
				continue ;
			}
		}
		i = 0;
		int s_num_q = 0;
		int d_num_q = 0;
		while (test.input[i])
		{
			if (test.input[i] == '\'')
				s_num_q++;
			if(test.input[i] == '\"')
				d_num_q++;
			i++;
		}
		if(s_num_q % 2 != 0 || d_num_q % 2 != 0)
		{
			ft_printf("%s: syntax error near unexpected token `newline'\n", test.name_program);
			test.exit_status = 2;
			continue ;
		}
		if (test.dir_count != test.operate_count)
		{
			ft_printf("%s: syntax error near unexpected token `newline'\n", test.name_program);
			test.exit_status = 2;
			continue ;
		}
		run_command(&test);
		free_shell(&test, 0, 1);
	}
	rl_clear_history();
	free_shell(&test, 1, 1);
	e_exit(&test, 0);
	return (0);
}

