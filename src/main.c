/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhalil <ikhalil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/09 20:08:46 by ikhalil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


#include "minishell.h"

int main(void)
{
    t_shell test;

    /* Set up signal handlers */
    signal(SIGINT, handle_signals);   // Handle Ctrl+C => New line
    signal(SIGQUIT, SIG_IGN);           // Ignore Ctrl+ /

    /* Initialize environment */
    ft_env_init(&test);

    while (1)
    {   
        /* Read input from user */
        test.input = readline("welcome to (shell)> ");
        if (!test.input)
        {
            printf("exit\n");  // Handle Ctrl+D => Exit shell
            break;
        }
        if (!test.input[0])
        {
            free(test.input);
            continue;
        }

        ft_printf("test: %s\n", test.input);

        /* Split the input into tokens (for example, using your ft_split) */
        test.input_splitted = ft_split(test.input);

        /* ----- Variable Expansion Step ----- */
        {
            int idx = 0;
            while (test.input_splitted[idx])
            {
                char *expanded = expand_token(test.input_splitted[idx], &test);
                free(test.input_splitted[idx]);
                test.input_splitted[idx] = expanded;
                idx++;
            }
        }
        /* ----------------------------------- */

        /* Further processing: e.g., split into commands, count tokens, etc. */
        test.split_the_split = split_commands(test.input_splitted);
        command_count(&test);
        operate(&test);
        dir(&test);

        /* Debug print of directory tokens */
        {
            int i = 0;
            while (test.dir && test.dir[i])
            {
                int j = 0;
                while (test.dir[i][j])
                {
                    ft_printf("dir[%d][%d]: %s\n", i, j, test.dir[i][j]);
                    j++;
                }
                i++;
            }
        }

        /* Execute built-in commands or external commands */
        if (test.command && test.command[0] && 
            strcmp(test.command[0][0], "cd") == 0 && test.command_count == 1)
            ft_cd(&test);
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "export") == 0)
            ft_export(&test, test.command[0]);
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "unset") == 0)
            ft_unset(&test, test.command[0]);
        else
            execute_pipeline(&test);

        add_history(test.input);
        free(test.input);

        /* Free allocated tokens and arrays as appropriate to avoid memory leaks */
    }
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