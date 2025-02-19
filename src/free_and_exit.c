#include "minishell.h"

void free_2d(char ***ar)
{
    int i = 0;

    if (!ar || !*ar)
        return;
    while ((*ar)[i])
    {
        free((*ar)[i]);
        i++;
    }
    free(*ar);
    *ar = NULL;
}

void free_3d(char ****ar)
{
    int i = 0;

    if (!ar || !*ar)
        return;
    while ((*ar)[i])
    {
        free_2d(&((*ar)[i]));
        i++;
    }
    free(*ar);
    *ar = NULL;
}

void free_shell(t_shell *shell, int i, int f, int child)
{
    if (child)
    {
        if (shell->input)
        {
            free(shell->input);
            if (shell->input_splitted)
            {
                free_3d(&shell->split_the_split);
                free_2d(&shell->input_splitted);
                int x = 0;
                while (shell->operate[x++])
                    free(shell->operate[x]);
                x = 0;
                while (shell->dir[x++])
                    free(shell->dir[x]);
                free(shell->command);
                shell->input = NULL;
                shell->input_splitted = NULL;
                shell->split_the_split = NULL;
                shell->command = NULL;
                shell->dir = NULL;
                shell->operate = NULL;
                shell->command_count = 0;
                shell->dir_count = 0;
                shell->operate_count = 0;
                shell->count_pipe = 0;
            }
        }
    }
    if(f)
    {
        free_2d(&shell->env);
        shell->env = NULL;
        if (shell->old_pwd)
        {
            free(shell->old_pwd);
            shell->old_pwd = NULL;
        }
        shell->exit_status = i;
        exit (i);
    }
}
