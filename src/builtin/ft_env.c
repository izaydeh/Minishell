#include "minishell.h"

extern char **environ;
void ft_env()
{
    int i = 0;
    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }
}