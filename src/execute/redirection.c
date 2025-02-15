#include "minishell.h"


int handle_input_redirection(char *filename, int *in_fd)
{
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    *in_fd = open(filename, O_RDONLY);
    if (*in_fd < 0) 
    {
        perror("error opening input file");
        return 0;
    }
    return 1;
}

int handle_output_redirection(const char *filename, int *out_fd, int i)
{
    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);
    if (i)
        *out_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        *out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (*out_fd < 0)
    {
        perror("error opening output file");
        return 0;
    }
    return 1;
}

int handle_here_doc(char *here_doc_world, int *in_fd) 
{
    char *line;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe in here_doc\n");
        return 0;
    }
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            perror("error in readline in here_doc:\n");
            break;
        }
        if (strcmp(line, here_doc_world) == 0)
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    *in_fd = pipe_fd[0];
    if (!line)
    {
        close(*in_fd);
        *in_fd = STDIN_FILENO;
        return 0;
    }
    return 1;
}

int check_operator(char *filename, char *operator, int *in_fd, int *out_fd)
{
    int success = 1;

    if (strcmp(operator, "<") == 0)
        success = handle_input_redirection(filename, in_fd);
    else if (strcmp(operator, ">") == 0)
        success = handle_output_redirection(filename, out_fd, 0);
    else if (strcmp(operator, ">>") == 0)
        success = handle_output_redirection(filename, out_fd, 1);
    else if (strcmp(operator, "<<") == 0)
        success = handle_here_doc(filename, in_fd);
    else 
    {
        perror("operator get_redirection error");
        success = 0;
    }
    return success;
}

int get_redirections(char *filename, char *operator, int *in_fd, int *out_fd)
{
    int success;

    if (!filename || !operator)
    {
        perror("error in redirection or file name");
        return 0;
    }
    success = check_operator(filename, operator, in_fd, out_fd);
    if (!success)
    {
        if (*in_fd != STDIN_FILENO) 
        {
            close(*in_fd);
            *in_fd = STDIN_FILENO;
        }
        if (*out_fd != STDOUT_FILENO)
        {
            close(*out_fd);
            *out_fd = STDOUT_FILENO;
        }
    }
    return success;
}
