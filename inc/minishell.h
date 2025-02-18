/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhalil <ikhalil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:48:58 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/18 10:34:29 by ikhalil          ###   ########.fr       */
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
	char	*old_pwd;
	int		command_count;
	int 	exit_status;
	int		operate_count;
	int		dir_count;
}			t_shell;

// struct for expander
typedef struct s_exp
{
	char	*res;
	char	*out;
}	t_exp;

//utils.c
int 	count_max_operate(t_shell *test, int y);
void 	operate(t_shell *test);//operate
int 	count_max_dir(t_shell *test, int y);
void 	dir(t_shell *test);//directoin
int 	count_max_commands(t_shell *test);
void 	command_count(t_shell *test);//commands

//./bulitin folder
int		is_builtin_funcion(t_shell *test, int i);
void 	run_builtin_function(t_shell *shell, int i, int out_fd);
void 	ft_cd(t_shell *path, int i);
void 	ft_echo(t_shell *string, int i);
void 	ft_env(t_shell *test);
void 	ft_env_init(t_shell *test);
void 	ft_pwd();
void	ft_export(t_shell *test, char **args);
void	ft_unset(t_shell *test, char **args);
void 	get_old_pwd(t_shell *shell);
char	*get_env_value(t_shell *shell, char *var);
void 	update_pwd(t_shell *shell);
void 	re_shlvl(t_shell *shell);

//execve.c
char	*getpath(t_shell *shell, char **test);

//handel_signals
void    handle_signals(int  sig);

//pipe.c
void 	execute_pipeline(t_shell *shell);

//redirection.c
int 	handle_input_redirection(char *filename, int *in_fd);
int 	handle_output_redirection(const char *filename, int *out_fd, int append);
int 	handle_here_doc(char *here_doc_world, int *in_fd) ;
int 	check_operator(char *filename, char *operate, int *in_fd, int *out_fd);
int 	get_redirections(char *filename, char *operator, int *in_fd, int *out_fd);

//expander
char	*expander(char *token, t_shell *shell);
char	*delete_spaces(char *s);
char	*process_dollar(char *s, t_shell *shell, t_exp *exp);
char	*handle_signal_quote(char *s, t_exp *exp);
char	*handle_double_quote(char *s, t_shell *shell, t_exp *exp);
char	*handle_un_quotetd(char *s, t_shell *shell, t_exp *exp);
int		should_delete_sp(char *token);


//free_and_exit
void free_shell(t_shell *shell, int i, int f);
void 	free_3d(char ***ar);
void 	free_2d(char **ar);

#endif
