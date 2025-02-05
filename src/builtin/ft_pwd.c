#include "minishell.h"

void ft_pwd()
{
    char path[1024];
    if (getcwd(path, sizeof(path)))
        printf("%s\n", path);
    else
        perror ("there is error in pwd"); 
}