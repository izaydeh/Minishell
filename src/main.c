/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhalil <ikhalil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/17 22:01:11 by ikhalil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(void)
{
    t_shell test;
    ft_bzero(&test, sizeof(t_shell));
    signal(SIGINT, handle_signals);   // Handle Ctrl+C => New line
    signal(SIGQUIT, SIG_IGN);         // Ignore Ctrl+ /
    ft_env_init(&test);
    re_shlvl(&test);

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
            free(test.input);
            continue;
        }
        
        /* First split the input by operators (ft_split does that) */
        test.input_splitted = ft_split(test.input);
        /* Then split the commands properly (echo gets special treatment) */
        test.split_the_split = split_commands(test.input_splitted);

        /* ----- Variable Expansion Step on Each Token ----- */
        int i = 0;
        while (test.split_the_split[i])
        {
            int j = 0;
            while (test.split_the_split[i][j])
            {
                char *expanded = expand_token(test.split_the_split[i][j], &test);
                free(test.split_the_split[i][j]);
                test.split_the_split[i][j] = expanded;
                j++;
            }
            i++;
        }

        command_count(&test);
        operate(&test);
        dir(&test);
        
        if (test.command && test.command[0] && 
            strcmp(test.command[0][0], "cd") == 0 && test.command_count == 1)
            ft_cd(&test, 0);
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "export") == 0)
            ft_export(&test, test.command[0]);
        else if (test.command && test.command[0] && 
                 strcmp(test.command[0][0], "unset") == 0)
            ft_unset(&test, test.command[0]);
        else
            execute_pipeline(&test);

        add_history(test.input);
        /* free_shell(&test, 0, 0); */
    }
    /* free_shell(&test, 0, 1); */
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