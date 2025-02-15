/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:24:17 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/13 00:18:02 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *getpath(t_shell *shell,char **test)
{
    if (!test || !test[0])
        return NULL;
    if (strchr(test[0], '/')) 
    {
        if (access(test[0], X_OK) == 0)
            return strdup(test[0]);
        return NULL;
    }
    char *path = get_env_value(shell,"PATH");
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
