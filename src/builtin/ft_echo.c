#include "minishell.h"

void ft_echo(t_shell *string, int i)
{
    int j = 0;
    if (string->command[i][1] == NULL && ft_strcmp(string->command[i][0], "echo") == 0)
        write(1, "\n", 1);
    else if (ft_strcmp(string->command[i][1], "-n") == 0)
    {
        while (string->command[i][2][j])
        {
            write(1, &string->command[i][2][j], 1);
            j++;
        }
    }
    else if (ft_strcmp(string->command[i][1], "-n") != 0)
    {
        while (string->command[i][1][j])
        {
            write(1, &string->command[i][1][j], 1);
            j++;
        }
        write(1, "\n", 1);
    }
    else
        perror("there is error in ehco");
}