/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:21:11 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/18 14:37:32 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_dir(t_shell *shell)
{
    struct stat test;
    int i = 0;
    if (stat(shell->input_splitted[i], &test) == 0 && S_ISDIR(test.st_mode))
        return 1;
    return 0;
}