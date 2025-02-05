/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhalil <ikhalil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:48:58 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/05 17:38:51 by ikhalil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "../libft/inc/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
#include <errno.h>
#include <sys/wait.h>

typedef struct s_shell
{
	char	*input;
	char	**input_splitted;
	char	***split_the_split;
}			t_shell;

void    execve_code(t_shell *s, char **envp, int i);
void    pipp(int argc, char **argv, char **envp, t_shell *test);
int is_builtin_funcion(t_shell *test);
void	ft_cd(t_shell *path);
void	ft_echo(t_shell *string);
void 	ft_env();
void	ft_pwd();
char *getpath(char **test);
void    handle_signals(int  sig);


#endif
