#include "minishell.h"

void re_shlvl(t_shell *shell)
{
    int level;
    char *shlvl;
    char *new_shlvl;
    char *shlvl_with_prefix;
    char *export[3];

    shlvl = get_env_value(shell, "SHLVL");
    level = 1;
    if (shlvl)
        level = ft_atoi(shlvl) + 1;
    new_shlvl = ft_itoa(level);
    if (!new_shlvl)
        return;
    shlvl_with_prefix = malloc(strlen("SHLVL=") + strlen(new_shlvl) + 1);
    if (!shlvl_with_prefix) 
    {
        free(new_shlvl);
        return;
    }
    strcpy(shlvl_with_prefix, "SHLVL=");
    strcat(shlvl_with_prefix, new_shlvl);
    export[0] = "export";
    export[1] = shlvl_with_prefix;
    export[2] = NULL;
    ft_export(shell, export);
    free(new_shlvl);
    free(shlvl_with_prefix);
}
