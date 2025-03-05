/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:59:18 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/27 20:09:36 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	pid = 0;
	if (!shell || !shell->split_the_split )
	{
		perror("Error: Invalid shell structure or environment");
		return ;
	}
	if (shell->command_count < 1)
	{
		in_fd = STDIN_FILENO;
	        out_fd = STDOUT_FILENO;
		int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);
		if (shell->count_pipe)
		{
			if (shell->count_pipe < shell->dir_count)
			{
				shell->dir_count -= 1;
				while (shell->dir_count--)
				{
					int b = 0;
        			while (shell->dir[a][b])
        			{
        				if(!get_redirections(shell->dir[a][b], shell->operate[a][b], &in_fd, &out_fd))
							shell->exit_status = 1;
						if (in_fd != STDIN_FILENO)
                            close(in_fd);
                        if (out_fd != STDOUT_FILENO)
                            close(out_fd);
						dup2(saved_stdin, STDIN_FILENO);
                        dup2(saved_stdout, STDOUT_FILENO);
						b++;
					}
					a++;
				}
			}
			else
			{
				write(2, "syntax error\n", 13);
				free_shell(shell, 0, 1);
                close(saved_stdin);
                close(saved_stdout);
				return ;
			}
		}
		else
		{
			int b = 0;
        	while (shell->dir[a][b])
        	{
        		if (!get_redirections(shell->dir[a][b], shell->operate[a][b], &in_fd, &out_fd))
					shell->exit_status = 1;
				if (in_fd != STDIN_FILENO)
                    close(in_fd);
                if (out_fd != STDOUT_FILENO)
                    close(out_fd);
				dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
				b++;
			}
		}
		close(saved_stdin);
        close(saved_stdout);
	}
	else 
	{
		while (i == 0 || i <= shell->count_pipe)
		{
	        in_fd = STDIN_FILENO;
	        out_fd = STDOUT_FILENO;
	        pipe_created = 0;
			argv = shell->command[i];
	        int redir_success = 1;
	        if (i > 0 && prev_fd != -1)
	            in_fd = prev_fd;
			if (a <= shell->count_pipe)
	        {
	            int b = 0;
	            while (shell->dir[a][b])
	            {
	                if (!(get_redirections(shell->dir[a][b], shell->operate[a][b], &in_fd, &out_fd)))
						redir_success = 0;
					else
						shell->exit_status = 1;
	                b++;
	            }
	        }
	        a++;
			if (i < shell->count_pipe)
			{
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe");
					free_shell(shell, 1, 1);
					e_exit(shell, 1);
				}
				pipe_created = 1;
			}
			pid = fork();
			if (pid < 0)
			{
				perror("fork");
				free_shell(shell, 1, 1);
				e_exit(shell, 1);
			}
			if (pid == 0)
			{
				signal(SIGQUIT, SIG_DFL);
		        if (!redir_success)
		        {
		            if (in_fd != STDIN_FILENO)
		                close(in_fd);
		            if (out_fd != STDOUT_FILENO)
		                close(out_fd);
					free_shell(shell, 1, 0);
					e_exit(shell, 1);
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
					if (strcmp(shell->command[i][0], "exit") == 0)
					{
        				ft_exit(shell, i);
						free_shell(shell, 1, 0);
						e_exit(shell, shell->exit_status);
					}
					free_shell(shell, 1, 0);
					e_exit(shell, shell->exit_status);
				}
				if (strcmp(shell->command[i][0], "/") == 0 && shell->command[i][1] == NULL)
				{
					print_error(shell->name_program, argv[0], "Is a directory");
					free_shell(shell, 1, 0);
					e_exit(shell, 126);
				}
				if (argv[0][strlen(argv[0]) - 1] == '/')
				{
					argv[0][strlen(argv[0]) - 1] = '\0';
					int dir_check = check_dir(argv[0]);
					if (dir_check == 1)
					{
						print_error(shell->name_program, argv[0], "is a directory");
						free_shell(shell, 1, 0);
						e_exit(shell, 126);
					}
					else if (dir_check == 3)
					{
						print_error(shell->name_program, argv[0], "Permission denied");
						free_shell(shell, 1, 0);
						e_exit(shell, 126);
					}
					else if (dir_check == 2)
					{
						print_error(shell->name_program, argv[0], "Not a directory");
						free_shell(shell, 1, 0);
						e_exit(shell, 126);
					}
					else
					{
						print_error(shell->name_program, argv[0], "No such file or directory");
						free_shell(shell, 1, 0);
						e_exit(shell, 127);
					}
				}
				cmd_path = getpath(shell, argv);
				if (!cmd_path || strcmp(argv[0], "..") == 0 || strcmp(argv[0], ".") == 0)
				{
					if (strcmp(argv[0], ".") == 0)
					{
						print_error(shell->name_program, argv[0], "filename argument required");
						free(cmd_path);
						free_shell(shell, 1, 0);
						e_exit(shell, 2);
					}
					else
					{
						print_error(shell->name_program, argv[0], "Command not found");
						free(cmd_path);
						free_shell(shell, 1, 0);
						e_exit(shell, 127);
					}
				}
				argv[0] = cmd_path;
				if (execve(cmd_path, argv, shell->env) == -1)
				{
					if (errno == EACCES)
					{
						print_error(shell->name_program, argv[0], "Permission denied");
						free(cmd_path);
						free_shell(shell, 1, 0);
						e_exit(shell, 126);
					}
					else if (errno == ENOEXEC)
					{
						print_error(shell->name_program, argv[0], "Exec format error");
						free(cmd_path);
						free_shell(shell, 1, 0);
						e_exit(shell, 126);
					}
					else
					{
						print_error(shell->name_program, argv[0], strerror(errno));
						free(cmd_path);
						free_shell(shell, 1, 0);
						e_exit(shell, 127);
					}
				}
			}
	        if (in_fd != STDIN_FILENO)
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
	}
	int cpid = 1;
	while (cpid > 0)
	{
		cpid = wait(&wstatus);
		if (cpid == pid)
		{
			if (WIFEXITED(wstatus))
				shell->exit_status = WEXITSTATUS(wstatus);
			else
				shell->exit_status = 128 + WTERMSIG(wstatus);
		}
	}
}
