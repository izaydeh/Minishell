/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:59:18 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/05 16:39:24 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    child_process(t_shell *test, char **envp ,int *i)
{
    pid_t pid;
    int fd[2];
    if (pipe(fd) == -1)
        perror("there is error in pipe:\n");
    pid = fork();
    if (pid == -1)
        perror("there is error in fork:\n");
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        execve_code(test, envp, i);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        waitpid(pid, NULL, 0);
    }
}

void    pipp(int argc, char **argv, char **envp, t_shell *test)
{
    int i;
    int infile;
    int outfile;

    i = 2;
    outfile = open_file (argv[argc - 1], 1);
    infile = open_file (argv[1], 2);
    dup2(infile, STDIN_FILENO);
    while (i < argc - 2)
    {
        // if (is_builtin_funcion(&test) == 0)
        if (test->input_splitted[i] == 0)
            child_process(&test, envp, i);
        i++;
    }
    dup2(outfile, STDOUT_FILENO);
    execve_code(&test, envp, argc - 2);
}