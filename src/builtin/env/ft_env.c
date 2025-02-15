#include "minishell.h"

void ft_env(t_shell *test)
{
    int i = 0;
    while (test->env[i])
    {    
        write (1, test->env[i], ft_strlen(test->env[i]));
        write(1, "\n", 1);
        i++;
    }
}

char *get_env_value(t_shell *shell, char *key)
{
    int i = 0;
    int len = ft_strlen(key);

    while (shell->env[i])
    {
        if (strncmp(shell->env[i], key, len) == 0 && shell->env[i][len] == '=')
            return shell->env[i] + len + 1;
        i++;
    }
    return NULL;
}

void ft_env_init(t_shell *test)
{
    extern char **environ;
    int i = 0;
    int j;
    while (environ[i])
        i++;
    test->env = malloc((i + 1) * sizeof(char *));
    if (!test->env)
        return;
    i = 0;
    while (environ[i])
    {
        test->env[i] = malloc((strlen(environ[i]) + 1) * sizeof(char));
        if (!test->env[i])
            return;
        j = 0;
        while (environ[i][j])
        {
            test->env[i][j] = environ[i][j];
            j++;    
        }
        test->env[i][j] = '\0';
        i++;
    }
    test->env[i] = NULL;
}