/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:48:58 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/03/02 04:36:12 by shoaib           ###   ########.fr       */
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
#include <sys/stat.h>

typedef struct s_shell
{
	char	*input;
	char	**input_splitted;
	char	***split_the_split;
	char	***command;
	char	***dir;
	char	***operate;
	char	**env;
	char	**exp;
	char	*old_pwd;
	char	*name_program;
	int		command_count;
	int 	exit_status;
	int		operate_count;
	int		dir_count;
	int		count_pipe;
	int 	flag_here;
}			t_shell;

extern int g_shell_signal;

// struct for expander
typedef struct s_exp
{
	char	*res;
	char	*out;
}	t_exp;

//dir.c
void	dir(t_shell *test);
void	process_dir_input(t_shell *test, int *j, int y);
void	store_redirect(t_shell *test, int *j, int y, int *x);
int		count_max_dir(t_shell *test, int y);

//operator.c
void	operate(t_shell *test);
void	process_input(t_shell *test, int *j, int y);
void	allocate_operate_array(t_shell *test);
int		count_max_operate(t_shell *test, int y);
void	count_pipe(t_shell *shell);

//command.c
int		count_max_commands(t_shell *test);
void	command_count(t_shell *test);
void	store_command(t_shell *test, int *j, int i);
void	process_commands(t_shell *test);

//./bulitin folder
int		is_builtin_funcion(t_shell *test, int i);
void 	run_builtin_function(t_shell *shell, int i, int out_fd);
void 	ft_cd(t_shell *path, int i);//cd
void	get_directory(t_shell *shell, int i, char **target);//cd
void 	ft_echo(t_shell *string, int i);//echo
void	handle_echo_flag(t_shell *string, int i, int *x, int *f);//echo
void	print_echo_arguments(t_shell *string, int i, int x);//echo
void 	ft_env(t_shell *test, int i);//env
void 	ft_env_init(t_shell *test);//env
void	ft_env_copy(t_shell *test, char **environ, int i);//env
char	*get_env_value(t_shell *shell, char *var);//env
void 	re_shlvl(t_shell *shell);//env
void	create_shlvl_entry(t_shell *shell, int level);//env
void 	ft_pwd();
void	ft_export(t_shell *test, char **args);
void	ft_unset(t_shell *test, char **args);
void 	get_old_pwd(t_shell *shell);
void 	update_pwd(t_shell *shell);
int 	is_numeric(const char *str);//exit
int 	check_argc_in_exit(char *argv);//exit
int		ft_exit(t_shell *shell, int i);//exit
int		validate_exit_arguments(t_shell *shell, int i);//exit

//split_command.c
int	tokenize_input(t_shell *g_shell, const char *input, char **tokens, int max_tokens, int *cmd_flags);

//split_utils.c
char	**ft_split_echo(char *s);
char	**parse_echo_arguments(char *s, char **tokens);
char	**ft_split_whitespace(char *s);
char	*extract_token_with_quotes(char **s);
size_t	ft_count_subwords(char *s);

//split_input.c
char	***split_commands(char **s);
char	***allocate_split(char **s, size_t *count);
void	skip_quoted_word(char **s);
char	**ft_split(const char *input, t_shell *shell);

//check_operator.c
int	is_redirection_operator(const char *op);
void	trim(char *str, char **start, char **end);
int	is_operator(const char *s, int *op_len);

//get_path.c
char	*getpath(t_shell *shell, char **test);
char	*find_executable(char *path, char *new_path, char *test);

//handel_signals
void    handle_signals(int  sig);

//pipe.c
void 	execute_pipeline(t_shell *shell);

// check_dir.c
int		check_dir(char *dir);
int 	msg_operate_error(t_shell *msg);

//redirection.c
int 	handle_input_redirection(char *filename, int *in_fd);
int 	handle_output_redirection(const char *filename, int *out_fd, int append);
int		handle_here_doc(char *here_doc_world, int *in_fd);
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
char	*remove_extra_spaces(char *s, char *new);
char	*process_character(char **s, t_shell *shell, t_exp *exp);
char	*process_token(char *s, t_shell *shell, t_exp *exp);
char	*d_remove_extra_spaces(char *s, char *new);
char	*d_delete_spaces(char *s);

//free_and_exit
void 	free_shell(t_shell *shell, int f, int child);
void	free_child_resources(t_shell *shell);
void	e_exit(t_shell *shell, int i);
void 	free_3d(char ***ar);
void 	free_2d(char **ar);
void	print_error(char *program, char *arg, char *msg);

#endif
