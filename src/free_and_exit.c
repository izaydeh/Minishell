#include "minishell.h"

void free_2d(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void free_3d(char ***array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free_2d(array[i]);
        i++;
    }
    free(array);
}

void free_shell(t_shell *shell, int i)
{
    free(shell->input);
    free(shell->old_pwd);
    free_2d(shell->input_splitted);
    free_2d(shell->env);
    free_3d(shell->split_the_split);
    free_3d(shell->command);
    free_3d(shell->dir);
    free_3d(shell->operate);
    shell->exit_status = i;
    exit (i);
}