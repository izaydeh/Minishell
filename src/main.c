/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/09 17:05:28 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main()
{
    t_shell test;

    signal(SIGINT,handle_signals); // Handle Ctrl + C => New line
    signal(SIGQUIT, SIG_IGN); // Hanlde Cntrl + \ => Do Nothing
    while (1)
    {   
        test.input = readline("welcome to (shell)>");
        if (!test.input)
        {
            printf("exit\n"); // Handle Ctrl + D => Exit Shell
            break;
        }
        if (!test.input[0])
            continue;
        if (test.input)
        {
            ft_printf("test: %s\n", test.input);
            test.input_splitted = ft_split(test.input);
            test.split_the_split = split_commands(test.input_splitted);
            command_count(&test);
            operate(&test);
            dir(&test);
            ft_env_init(&test);
            int i = 0;
            while (test.dir[i])
            {
                int j = 0;
                while (test.dir[i][j])
                {
                    ft_printf("dir[%d][%d]: %s\n", i, j, test.dir[i][j]);
                    j++;
                }
                i++;
            }
            if (strcmp(test.command[0][0], "cd") == 0 && test.command_count == 1)
                ft_cd(&test);
            else
                execute_pipeline(&test);
            add_history(test.input);
            free(test.input);
        }
    }
    return 0;
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