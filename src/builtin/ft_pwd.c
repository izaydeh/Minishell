/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:35:01 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 19:35:02 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char	path[1024];
	int		i;

	if (getcwd(path, sizeof(path)))
	{
		i = 0;
		while (path[i])
		{
			write(1, &path[i], 1);
			i++;
		}
		write(1, "\n", 1);
	}
	else
		perror("there is error in pwd");
}
