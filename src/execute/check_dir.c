/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:21:11 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/26 17:07:48 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir(char *dir)
{
	struct stat	test;

	if (stat(dir, &test) != 0)
	{
		if (errno == EACCES)
            return (3);
		return (0);
	}
	if (S_ISDIR(test.st_mode))
		return (1);
	if (S_ISREG(test.st_mode))
	{
		if (access(dir, X_OK) != 0)
			return (3);
		return (2);
	}
	return (0);
}

int	msg_operate_error(t_shell *msg)
{
	if (msg->dir_count == msg->operate_count)
		return (0);
	else
	{
		ft_printf("%s: syntax error near unexpected token `newline'\n",
				msg->name_program);
		return (1);
	}
}