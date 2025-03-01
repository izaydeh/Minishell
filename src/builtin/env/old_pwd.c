/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:33:05 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 19:33:06 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_old_pwd(t_shell *test)
{
	char	path[1024];
	char	*export[3];
	char	*oldpwd;
	int		oldpwd_len;

	if (getcwd(path, sizeof(path)) == NULL)
	{
		perror("getcwd error");
		return ;
	}
	oldpwd_len = strlen("OLDPWD=") + strlen(path) + 1;
	oldpwd = (char *)malloc(oldpwd_len);
	if (!oldpwd)
		return ;
	strcpy(oldpwd, "OLDPWD=");
	strcat(oldpwd, path);
	export[0] = "export";
	export[1] = oldpwd;
	export[2] = NULL;
	ft_export(test, export);
	free(oldpwd);
}
