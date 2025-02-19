/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:24:15 by ikhalil           #+#    #+#             */
/*   Updated: 2025/02/19 16:54:26 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_signals(int  sig)
{
    if (sig == SIGINT)
    {
        printf("\n");  // Move to a new line
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
        // Do Nothing :)
    }
}
