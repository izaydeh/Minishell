#include "minishell.h"

void update_pwd(t_shell *shell)
{
    char path[1024];
    char *export[3];
    char *pwd;
    int pwd_len;
    if (getcwd(path, sizeof(path)) == NULL)
    {
        perror("getcwd error");
        return;
    }
    pwd_len = strlen("PWD=") + strlen(path) + 1;
    pwd = (char *)malloc(pwd_len);
    if (!pwd)
        return;
    strcpy(pwd, "PWD=");
    strcat(pwd, path);

    export[0] = "export";
    export[1] = pwd;
    export[2] = NULL;
    ft_export(shell, export);
    free(pwd);
}
