/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:59:18 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/09 16:09:36 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_str_array(char **array)
{
    int i = 0;

    if (!array)
        return;

    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
// expander(&argv); 
///////////////////////////////////////////////////
//for builtin functions ===>
// if (builtins(argv) && (*shell)->parser->command_count == 1) {
//     exec_builtins(argv, *shell);
//     free_str_array(argv);
//     return;
// }
//anther way from chatgpt==>
// if (is_builtin(argv) && shell->command_count == 1)
// {
//     shell->exit_status = exec_builtins(argv, shell);
//     return;
// }
///////////////////////////////////////////////////static 

void execute_pipeline(t_shell *shell)
{
 	int		i;
	int		prev_fd;
	int		pipe_fd[2];
	int		pid;
	int		in_fd;
	int		out_fd;
	int		pipe_created;
	int		wstatus;
	char	*cmd_path;
	char	**argv;
    int     a;

    a = 0;
	i = 0;
	prev_fd = -1;
	if (!shell || !shell->split_the_split )
	{
		perror("Error: Invalid shell structure or environment");
		return ;
	}
	while (i < shell->command_count)
	{
        in_fd = STDIN_FILENO;
        out_fd = STDOUT_FILENO;
        pipe_created = 0;
		argv = shell->command[i];
        int redir_success = 1;
		if (a < shell->command_count)
        {
            int b = 0;
            while (shell->dir[a][b] != NULL)
            {
                if (!(get_redirections(shell->dir[a][b], shell->operate[a][b], &in_fd, &out_fd)))
                    redir_success = 0;
                b++;
            }
        }
        a++;
        if (i > 0 && prev_fd != -1)
            in_fd = prev_fd;
		if (i < shell->command_count - 1)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);//1
			}
			pipe_created = 1;
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);//1
		}
		if (pid == 0)
		{
        	if (!redir_success)
        	{
        	    if (in_fd != STDIN_FILENO)
        	        close(in_fd);
        	    if (out_fd != STDOUT_FILENO)
        	        close(out_fd);
        	    shell->exit_status = 1;
				exit(1);
        	}
			if (in_fd != STDIN_FILENO)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
			if (pipe_created)
			{
				if (out_fd == STDOUT_FILENO)
					out_fd = pipe_fd[1];
			}
            if (out_fd != STDOUT_FILENO)
            {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
			if (prev_fd != -1)
				close(prev_fd);
			if (pipe_created)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (is_builtin_funcion(shell, i))
			{
				run_builtin_function(shell, i, out_fd);
				if (in_fd != STDIN_FILENO)
					close(in_fd);
				if (out_fd != STDOUT_FILENO)
					close(out_fd);
				exit(0);
			}
			cmd_path = getpath(argv);
			if (!cmd_path)
			{
				perror(argv[0]);
				free_str_array(argv);
				shell->exit_status = 127;
				exit(127);
			}
			argv[0] = cmd_path;
			if (execve(cmd_path, argv, shell->env) == -1)
            {
				perror("execve");
	            free_str_array(argv);
	            free(cmd_path);
	            exit(EXIT_FAILURE);//126
			}
		}
        if (in_fd != STDIN_FILENO && in_fd != prev_fd)
            close(in_fd);
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		if (prev_fd != -1)
			close(prev_fd);
		if (pipe_created)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		else
			prev_fd = -1;
		i++;
	}
	while (wait(&wstatus) > 0)
	{
		if (WIFEXITED(wstatus))
			shell->exit_status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus))
			shell->exit_status = 128 + WTERMSIG(wstatus);
	}
}
//u i back