/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:24:15 by ikhalil           #+#    #+#             */
/*   Updated: 2025/02/21 19:36:07 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_signals(int  sig)
{
    if (sig == SIGINT)
    {
        if (g_shell)
            g_shell->exit_status = 130;
            
        printf("\n");  // Move to a new line
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
        if (g_shell)
            g_shell->exit_status = 0;
        // Do Nothing :)
    }
}
