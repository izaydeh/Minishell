#include "minishell.h"


void ft_echo(t_shell *string, int i)
{
    int j = 0;
    int x = 1;
    int f = 0;
    if (!string->command[i][1])
    {
        write(1, "\n", 1);
        return;
    }
    if (string->command[i][1][0] == '-' && string->command[i][1][1] == 'n')
    {
        while (string->command[i][1][x])
        {
            if (string->command[i][1][x] != 'n' && string->command[i][1][x] != 'e') 
            {
                f = 1;
                break;
            }
            x++;
        }
        if (f == 0)
        {
            x = 2;
            while (string->command[i][x])
            {
                j = 0;
                while (string->command[i][x][j])
                {
                    write(1, &string->command[i][x][j], 1);
                    j++;
                }
                if (string->command[i][x + 1])
                    write(1, " ", 1);
                x++;
            }
            return;
        }
    }
    x = 1;
    while (string->command[i][x])
    {
        j = 0;
        while (string->command[i][x][j])
        {
            write(1, &string->command[i][x][j], 1);
            j++;
        }
        if (string->command[i][x + 1])
            write(1, " ", 1);
        x++;
    }
    write(1, "\n", 1);
}
