#include "minishell.h"

int handle_input_redirection(char *filename, int *in_fd)
{
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    *in_fd = open(filename, O_RDONLY);
    if (*in_fd < 0) 
    {
        perror("Error opening input file");
        return 0;
    }
    return 1;
}

int handle_output_redirection(const char *filename, int *out_fd, int append)
{
    if (*out_fd != STDOUT_FILENO)
        close(*out_fd);
    if (append)
        *out_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        *out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (*out_fd < 0)
    {
        perror("Error opening output file");
        return 0;
    }
    return 1;
}

int handle_here_doc(const char *delimiter, int *in_fd) 
{
    char *line = NULL;
    size_t len = 0;
    int pipe_fd[2];
    ssize_t read;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return 0;
    }

    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    printf("> ");
    while ((read = getline(&line, &len, stdin)) != -1) 
    {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, delimiter) == 0)
            break;
        if (write(pipe_fd[1], line, strlen(line)) == -1)
        {
            perror("write error");
            break ;
        }
        write(pipe_fd[1], "\n", 1);
        printf("> ");
    }
    if (read == -1)
        fprintf(stderr, "minishell: warning: here-doc delimited by EOF (wanted `%s')\n", delimiter);
    free(line);
    close(pipe_fd[1]);
    *in_fd = pipe_fd[0];
    return 1;
}

int get_redirections(char *filename, char *operator, int *in_fd, int *out_fd)
{
    *in_fd = STDIN_FILENO;
    *out_fd = STDOUT_FILENO;
    int success = 1;

    if (!filename || !operator)
    {
        fprintf(stderr, "Redirection error: missing filename or operator\n");
        return 0;
    }
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
        fprintf(stderr, "Unknown redirection operator: %s\n", operator);
        success = 0;
    }
    if (!success)
    {
        if (*in_fd != STDIN_FILENO) 
        {
            close(*in_fd);
            *in_fd = STDIN_FILENO;
        }
        if(*out_fd != STDOUT_FILENO)
        {
            close(*out_fd);
            *out_fd = STDOUT_FILENO;
        }
    }
    return success;
}