/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:33:11 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 19:33:12 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_shlvl_entry(t_shell *shell, int level)
{
	char	*new_shlvl;
	char	*shlvl_with_prefix;
	char	*export[3];

	new_shlvl = ft_itoa(level);
	if (!new_shlvl)
		return ;
	shlvl_with_prefix = malloc(strlen("SHLVL=") + strlen(new_shlvl) + 1);
	if (!shlvl_with_prefix)
	{
		free(new_shlvl);
		return ;
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

void	re_shlvl(t_shell *shell)
{
	int		level;
	char	*shlvl;

	shlvl = get_env_value(shell, "SHLVL");
	if (!shlvl || !is_numeric(shlvl))
		level = 1;
	else
	{
		level = ft_atoi(shlvl) + 1;
		if (level < 0)
			level = 0;
		else if (level >= 1000)
		{
			printf("./minishell: warning: shell level (1000) too high, resetting to 1\n");
			level = 1;
		}
	}
	create_shlvl_entry(shell, level);
}
