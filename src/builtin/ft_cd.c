#include "minishell.h"

void ft_cd(t_shell *shell, int i)
{
    char *target = NULL;
    char *home = get_env_value(shell, "HOME");

    if (!shell->command[i][1] || strcmp(shell->command[i][1], "~") == 0 || strcmp(shell->command[i][1], "--") == 0)
    {
        if (!home)
        {
            perror("cd: HOME not set\n");
            shell->exit_status = 1;
            return;
        }
        target = home;
        printf("%s\n", target);
    }
    else
    {
        target = shell->command[i][1];
        if (check_dir(target) != 1)
        {
            write (2, "the argument not a directory \n", 31);
            return ;
        }
    }
    get_old_pwd(shell);
    if (chdir(target) != 0)
    {
        perror("cd");
        shell->exit_status = 1;
    }
    else
    {
        update_pwd(shell);
        shell->exit_status = 0;
    }
}
