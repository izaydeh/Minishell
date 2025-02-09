#include "minishell.h"

void ft_pwd()
{
    char path[1024];
    if (getcwd(path, sizeof(path)))
    {
        int i = 0;
        while (path[i])
        {
            write(1, &path[i], 1);
            i++;
        }
        write(1, "\n", 1);
    }
    else
        perror ("there is error in pwd");
}