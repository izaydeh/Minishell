/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/21 15:53:25 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell *g_shell = NULL;

int main(int argc,char **argv)
{
    (void)argc;
    t_shell test;
    ft_bzero(&test, sizeof(t_shell));
    g_shell = &test;
    signal(SIGINT, handle_signals);   // Handle Ctrl+C => New line
    signal(SIGQUIT, SIG_IGN);         // Ignore Ctrl+ /
    ft_env_init(&test);
    re_shlvl(&test);
    test.name_program = argv[0];
    while (1)
    {
        test.input = readline("welcome to (shell)> ");
        if (!test.input)
        {
            write(2, "exit\n", 5);
            break;
        }
        if (!test.input[0])
        {
            free_shell(&test, 0, 0, 1);
            continue;
        }
        test.input_splitted = ft_split(test.input);
        test.split_the_split = split_commands(&test, test.input_splitted);
        int i = 0;
        while (test.split_the_split[i])
        {
            int j = 0;
            while (test.split_the_split[i][j])
            {
                char *expanded = expander(test.split_the_split[i][j], &test);
                free(test.split_the_split[i][j]);
                test.split_the_split[i][j] = expanded;
                j++;
            }
            i++;
        }
        command_count(&test);
        count_pipe(&test);
        operate(&test);
        dir(&test);
        if (test.command_count == 0 && test.count_pipe == 0
            && test.dir_count == 0)
        {
            if (msg_operate_error(&test))
                test.exit_status = 2;
        }
        else if (test.command_count == 1 && test.count_pipe == 0 &&
                test.input_splitted[0][strlen(test.input_splitted[0]) - 1] == '/')
        {
            if (check_dir(test.input_splitted[0]) == 1)
                printf("%s is a directory.\n", test.input_splitted[0]);
        }
        else if (test.command && test.command[0] && 
            strcmp(test.command[0][0], "cd") == 0 && test.command_count == 1)
            ft_cd(&test, 0);
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "export") == 0)
            ft_export(&test, test.command[0]);
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "exit") == 0)
            {
                if (ft_exit(&test, 0))
                    exit(test.exit_status);
            }
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "unset") == 0)
            ft_unset(&test, test.command[0]);
        else
            execute_pipeline(&test);

        add_history(test.input);
        free_shell(&test, 0, 0, 1);
    }
    free_shell(&test, 0, 1, 1);
    return (0);
}

// while (test.input_splitted[i])
// {
//     ft_printf("test_splted[%d]: %s\n", i, test.input_splitted[i]);
//     i++;
// }
// while (test.operate[i])
// {
//     ft_printf("operate[%d]: %s\n", i, test.operate[i]);
//     i++;
// }
// i = 0;
// i = 0;
// while (test.split_the_split[i])
// {
//     int j = 0;
//     while (test.split_the_split[i][j])
//     {
//         ft_printf("split_the_split[%d][%d]: %s\n", i, j, test.split_the_split[i][j]);
//         j++;
//     }
//     i++;
// }