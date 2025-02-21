#include "minishell.h"


void ft_echo(t_shell *string, int i)
{
    int j = 0;
    int x = 1;
    int f = 0;
    if (!string->split_the_split[i][1])
    {
        write(1, "\n", 1);
        return;
    }
    if (string->split_the_split[i][1][0] == '-' && string->split_the_split[i][1][1] == 'n')
    {
        while (string->split_the_split[i][1][x])
        {
            if (string->split_the_split[i][1][x] != 'n' && string->split_the_split[i][1][x] != 'e') 
            {
                f = 1;
                break;
            }
            x++;
        }
        if (f == 0)
        {
            x = 2;
            while (string->split_the_split[i][x])
            {
                j = 0;
                while (string->split_the_split[i][x][j])
                {
                    write(1, &string->split_the_split[i][x][j], 1);
                    j++;
                }
                if (string->split_the_split[i][x + 1])
                    write(1, " ", 1);
                x++;
            }
            return;
        }
    }
    x = 1;
    while (string->split_the_split[i][x])
    {
        j = 0;
        while (string->split_the_split[i][x][j])
        {
            write(1, &string->split_the_split[i][x][j], 1);
            j++;
        }
        if (string->split_the_split[i][x + 1])
            write(1, " ", 1);
        x++;
    }
    write(1, "\n", 1);
}
