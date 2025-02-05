#include "minishell.h"

void ft_echo(t_shell *string)
{
    if (string->split_the_split[0][1] == NULL && ft_strcmp(string->split_the_split[0][0], "echo") == 0)
        write(1, "\n", 1);
    else if (ft_strcmp(string->split_the_split[0][1], "-n") != 0)
        printf("%s\n",string->split_the_split[0][1]);
    else if (ft_strcmp(string->split_the_split[0][1], "-n") == 0)
        printf("%s",string->split_the_split[0][2]);
    else
    {
        printf("%s", "there is error in ehco");
        return ;
    }
}