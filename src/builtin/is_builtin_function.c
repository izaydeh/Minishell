#include "minishell.h"

int is_builtin_funcion(t_shell *test)
{
    if (ft_strcmp(test->split_the_split[0][0], "cd") == 0)
    {
        ft_cd(test);
        return 1;
    }
    else if (ft_strcmp(test->split_the_split[0][0], "echo") == 0)
    {
        ft_echo(test);
        return 1;
    }
    else if (ft_strcmp(test->split_the_split[0][0], "pwd") == 0)
    {
        ft_pwd();
        return 1;
    }
    else if (ft_strcmp(test->split_the_split[0][0], "env") == 0)
    {
        ft_env();
        return 1;
    }
    // else if (ft_strcmp(test->split_the_split[0][0], "unset"))
    // {
    //     ft_unset(&test);
    //     return 1;
    // }
    // else if (ft_strcmp(test->split_the_split[0][0], "export") == 0)
    // {
    //      ft_export(test->split_the_split[0][1]);
    //      return 1;
    // }
    else
        return 0;
}