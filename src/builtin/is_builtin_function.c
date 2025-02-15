#include "minishell.h"

int is_builtin_funcion(t_shell *test, int i)
{
    if (strcmp(test->command[i][0], "cd") == 0 || strcmp(test->command[i][0], "pwd") == 0 || strcmp(test->command[i][0], "env") == 0 ||
        strcmp(test->command[i][0], "echo") == 0 || strcmp(test->command[i][0], "unset") == 0 ||
        strcmp(test->command[i][0], "export") == 0)
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
    int stdout = dup(STDOUT_FILENO);
    // int stdin = dup(STDIN_FILENO);

    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
    // if (in_fd != STDIN_FILENO)
    // {
    //     dup2(in_fd, STDIN_FILENO);
    //     close(in_fd);
    // }
    if (strcmp(shell->command[i][0], "unset") == 0)
        ft_unset(shell, shell->command[i]);
    else if (strcmp(shell->command[i][0], "export") == 0)
        ft_export(shell, shell->command[i]);
    else if (ft_strcmp(shell->command[i][0], "echo") == 0)
        ft_echo(shell, i);
    else if (ft_strcmp(shell->command[i][0], "cd") == 0)
        ft_cd(shell, i);
    else if (ft_strcmp(shell->command[i][0], "env") == 0)
        ft_env(shell);
    else if (ft_strcmp(shell->command[i][0], "pwd") == 0)
        ft_pwd();

    dup2(stdout, STDOUT_FILENO);
    close(stdout);
    // dup2(stdin, STDIN_FILENO);
    // close(stdin);
}
