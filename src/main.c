/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:46:28 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/05 16:37:09 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    t_shell test;
    int i;
    while (1)
    {   
        test.input = readline("welcome to بهجت صابر (shell)>");
        if (test.input)
        {
            ft_printf("test: %s\n", test.input);
            test.input_splitted = ft_split(test.input);
            i = 0;
            while (test.input_splitted[i])
            {
                ft_printf("test_splted[%d]: %s\n", i, test.input_splitted[i]);
                i++;
            }
            i = 0;
            test.split_the_split = split_commands(test.input_splitted);
            while (test.split_the_split[i])
            {
                int j = 0;
                while (test.split_the_split[i][j])
                {
                    ft_printf("split_the_split[%d][%d]: %s\n", i, j, test.split_the_split[i][j]);
                    j++;
                }
                i++;
            }
            i = 0;
            if (is_builtin_funcion(&test) == 0)
                pipp(argc, argv, envp, &test);
            add_history(test.input);
            free(test.input);
        }
    }
    return 0;
}