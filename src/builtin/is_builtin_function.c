#include "minishell.h"

int is_builtin_funcion(t_shell *test, int i)
{
    if (strcmp(test->command[i][0], "cd") == 0 || strcmp(test->command[i][0], "pwd") == 0 || strcmp(test->command[i][0], "env") == 0 || strcmp(test->command[i][0], "echo") == 0)
        return 1;
    return 0;
}
// void run_builtin_function(t_shell *test, int i, int in_fd, int out_fd)
// {
//     (void)out_fd;
//     if (ft_strcmp(test->command[i][0], "echo") == 0)
//         ft_echo(test, i, in_fd);
//     else if (ft_strcmp(test->command[i][0], "pwd") == 0)
//         ft_pwd(in_fd);
//     else if (ft_strcmp(test->command[i][0], "env") == 0)
//         ft_env(in_fd);
// }

void run_builtin_function(t_shell *shell, int i, int out_fd)
{
    int stdout_backup = dup(STDOUT_FILENO);
    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }

    if (ft_strcmp(shell->command[i][0], "echo") == 0)
        ft_echo(shell, i);
    else if (ft_strcmp(shell->command[i][0], "cd") == 0)
        ft_cd(shell);
    else if (ft_strcmp(shell->command[i][0], "env") == 0)
        ft_env(shell);
    else if (ft_strcmp(shell->command[i][0], "pwd") == 0)
        ft_pwd();

    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
}
