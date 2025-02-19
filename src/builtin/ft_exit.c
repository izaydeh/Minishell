/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:47:55 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/19 17:27:00 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_numeric(const char *str)
{
    int i = 0;
    if (!str || !str[i])
        return (0);
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i] && str[i] != ' ')
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int check_argc_in_exit(char *argv)
{
    int i = 0;
    while (argv[i])
    {
        if (argv[i] == 32)
            return 0;
        i++;
    }
    return 1;
}

int ft_exit(t_shell *shell, int i)
{
    long exit_int;

    printf("exit\n");
    if (!shell->command[i])//exit without argumant
    {
        shell->exit_status = 0;
        return 1;
    }
    if (!is_numeric(shell->command[i][1]))//if there is '-' or alphabit
    {
        printf("%s: ft_exit: %s: numeric argument required\n", shell->name_program, shell->command[i][1]);
        shell->exit_status = 2;
        return 1;
    }
    if (shell->command[i][2])//exit 1 2 3 with many argumant
    {
        printf("%s: ft_exit: too many arguments\n", shell->name_program);
        return 0;
    }

    exit_int = ft_atoi(shell->command[i][1]);
    if (exit_int > 255)//if number > 256
    {
        shell->exit_status = exit_int % 256;
        return 1;
    }
    if (exit_int < 0)
    {
        shell->exit_status = (exit_int % 256 + 256) % 256;
        return 1;
    }
    shell->exit_status = exit_int;
    return 1;
}