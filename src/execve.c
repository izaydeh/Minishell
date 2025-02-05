/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:24:17 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/05 16:39:58 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *getpath(char **test)
{
    char *path = getenv("PATH");
    if (!path)
        return NULL;
    char *start = path;
    char *end = NULL;
    char *new_path = malloc(sizeof(char) * 1024);
    if (!new_path)
        return NULL;
    while((end = ft_strchr(path, ':')) != NULL)
    {
        int length = end - start;
        strncpy(new_path, start, length);
        new_path[length] = '\0';
        strcat(new_path, "/");
        strcat(new_path, test[0]);
        if (access(new_path, X_OK) == 0)
            return (new_path);
        path += length + 1;        
        start = end+1;
    }
    strcpy(new_path, start);
    strcat(new_path, "/");
    strcat(new_path, test[0]);
    if (access(new_path, X_OK) == 0)
        return (new_path);
    free(new_path);
    return NULL;
}
-
void    execve_code(t_shell *s, char **envp, int i)
{
    // pid_t pid;
    char *path;
    path = getpath(s->split_the_split[i]);
    if (!path)
    {
        write (1, "the command not found\n", 22);
        return ;
    }
    s->split_the_split[i][0] = path;
    int j = 0;
    while (s->split_the_split[i][j])
    {
        ft_printf("split_the_split[%d][%d]: %s\n", i, j, s->split_the_split[i][j]);
        j++;
    }

    // pid = fork();
    // if (pid == -1)
    // {
    //     write(1, "fork failed\n", 12);
    //     return;
    // }
    // else if (pid == 0)
    // {
    //     if (execve(path, s->split_the_split[0], envp) == -1)
    //     {
    //         write(1, "the command not found2\n", 23);
    //         free(path);
    //         exit(1);
    //     }
    // }
    // else
    //     waitpid(pid, NULL, 0);
    if (execve(path, s->split_the_split[i], envp) == -1)
    {
        write(1, "the command not found2\n", 23);
        free(path);
        exit(1);
    }
}
