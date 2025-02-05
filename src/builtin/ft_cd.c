#include "minishell.h"

void ft_cd(t_shell *path)
{
    if (chdir(path->split_the_split[0][1]) != 0 && path->split_the_split[0][1] != NULL)
        perror("the path in cd is error");
    if (path->split_the_split[0][1] == NULL)
    {
        char *home_dir = getenv("HOME");
        if (home_dir != NULL)
        {
            if (chdir(home_dir) != 0)
                perror("the path in cd is error");
        }
    
    }
}