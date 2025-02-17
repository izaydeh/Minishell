#include "minishell.h"

void free_2d(char **ar)
{
    int i = 0;
    if (!ar)
        return;
    while (ar[i])
    {
        free(ar[i]);
        i++;
    }
    free(ar);
}

void free_3d(char ***ar)
{
    int i = 0;
    if (!ar)
        return;
    while (ar[i])
    {
        free_2d(ar[i]);
        i++;
    }
    free(ar);
}

void free_shell(t_shell *shell, int i, int f)
{
    free(shell->input);
    free_2d(shell->input_splitted);
    free_3d(shell->split_the_split);
    free_3d(shell->command);
    free_3d(shell->dir);
    free_3d(shell->operate);
    shell->input_splitted = NULL;
    shell->split_the_split = NULL;
    shell->command = NULL;
    shell->dir = NULL;
    shell->operate = NULL;
    if(f)
    {
        free_2d(shell->env);
        free(shell->old_pwd);
        shell->exit_status = i;
        exit (i);
    }
}