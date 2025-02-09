#include "minishell.h"

void ft_cd(t_shell *path)
{
    if (path->command[0][1] != NULL)
    {
        if (chdir(path->command[0][1]) != 0)
            perror("the path in cd is error");
    }
    else
    {
        char *home_dir = getenv("HOME");
        if (home_dir != NULL)
        {
            if (chdir(home_dir) != 0)
                perror("the path in cd is error");
        }
    }
}