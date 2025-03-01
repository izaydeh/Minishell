/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:24:17 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/25 20:10:18 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getpath(t_shell *shell, char **test)
{
	char	*path;
	char	*new_path;
	char	*result;

	if (!test || !test[0])
		return (NULL);
	if (strchr(test[0], '/'))
	{
		if (access(test[0], X_OK) == 0)
			return (strdup(test[0]));
		return (NULL);
	}
	path = get_env_value(shell, "PATH");
	if (!path)
		return (NULL);
	new_path = malloc(sizeof(char) * 1024);
	if (!new_path)
		return (NULL);
	result = find_executable(path, new_path, test[0]);
	if (result)
		return (result);
	free(new_path);
	return (NULL);
}

char	*find_executable(char *path, char *new_path, char *test)
{
	char	*start;
	char	*end;
	int		length;

	start = path;
	end = NULL;
	while ((end = ft_strchr(path, ':')) != NULL)
	{
		length = end - start;
		strncpy(new_path, start, length);
		new_path[length] = '\0';
		strcat(new_path, "/");
		strcat(new_path, test);
		if (access(new_path, X_OK) == 0)
			return (new_path);
		path += length + 1;
		start = end + 1;
	}
	strcpy(new_path, start);
	strcat(new_path, "/");
	strcat(new_path, test);
	if (access(new_path, X_OK) == 0)
		return (new_path);
	return (NULL);
}
