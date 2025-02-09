/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:48:58 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/09 15:17:59 by sal-kawa         ###   ########.fr       */
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
#include <ctype.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/types.h>

typedef struct s_shell
{
	char	*input;
	char	**input_splitted;
	char	***split_the_split;
	char	***command;
	char	***dir;
	char	***operate;
	char	**env;
	int		command_count;
	int 	exit_status;
	int		operate_count;
	int		dir_count;
}			t_shell;

//utils.c
int 	count_max_operate(t_shell *test, int y);
void 	operate(t_shell *test);//operate
int 	count_max_dir(t_shell *test, int y);
void 	dir(t_shell *test);//directoin
int 	count_max_commands(t_shell *test);
void 	command_count(t_shell *test);//commands

//./bulitin folder
int		is_builtin_funcion(t_shell *test, int i);
void 	run_builtin_function(t_shell *test, int i, int out_fd);
void 	ft_cd(t_shell *path);
void 	ft_echo(t_shell *string, int i);
void 	ft_env(t_shell *test);
void 	ft_env_init(t_shell *test);
void 	ft_pwd();

//execve.c
char 	*getpath(char **test);

//handel_signals
void    handle_signals(int  sig);

//pipe.c
void 	execute_pipeline(t_shell *shell);

//redirection.c
int 	handle_input_redirection(char *filename, int *in_fd);
int 	handle_output_redirection(const char *filename, int *out_fd, int append);
int 	handle_here_doc(const char *delimiter, int *in_fd);
int 	get_redirections(char *filename, char *operator, int *in_fd, int *out_fd);

#endif
