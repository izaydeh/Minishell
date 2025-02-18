#include "minishell.h"

//----------------------------------------------------------operate ==> **operate
int count_max_operate(t_shell *test, int y)
{
    int count = 0;
    int i = 0;
    int command_index = 0;    
    if (!test->input_splitted)
        return 0;
    while (test->input_splitted[i]) 
    {
        if (strcmp(test->input_splitted[i], "|") == 0) 
            command_index++;
        else if (command_index == y)
        {
            if (strcmp(test->input_splitted[i], "<") == 0 || strcmp(test->input_splitted[i], ">") == 0 ||
                strcmp(test->input_splitted[i], "<<") == 0 || strcmp(test->input_splitted[i], ">>") == 0)
                count++;
        }
        i++;
    }
    return count;
}

void operate(t_shell *test)
{
    int j = 0;
    int y = 0;
    test->operate_count = 0;

    test->operate = malloc(sizeof(char **) * (test->count_pipe + 2));
    if (!test->operate)
        return;
    while (y < test->count_pipe + 1)
    {
        int x = 0;
        test->operate[y] = malloc(sizeof(char *) * (count_max_operate(test, y) + 1));
        if (!test->operate[y])
            return;
        while (test->input_splitted[j] && (strcmp(test->input_splitted[j], "|") != 0))
        {
            if (strcmp(test->input_splitted[j], "<") == 0 || strcmp(test->input_splitted[j], ">") == 0 ||
               strcmp(test->input_splitted[j], "<<") == 0 || strcmp(test->input_splitted[j], ">>") == 0)
            {
                    test->operate[y][x] = test->input_splitted[j];
                    if (!test->operate[y][x]) 
                    {
                        fprintf(stderr, "Memory allocation failed for operate[%d][%d]\n", y, x);
                        return;
                    }
                    test->operate_count++;
                    x++;
            }
            j++; 
        }
        test->operate[y][x] = NULL;
        if (test->input_splitted[j])
            j++;
        y++;
    }
    test->operate[y] = NULL;
}
//---------------------------------------------------------------directions ==> dir_count, ***dir
int count_max_dir(t_shell *test, int y)
{
    int count = 0;
    int i = 0;
    int command_index = 0;    
    if (!test->input_splitted)
        return 0;
    while (test->input_splitted[i]) 
    {
        if (strcmp(test->input_splitted[i], "|") == 0) 
            command_index++;
        else if (command_index == y)
        {
            if (strcmp(test->input_splitted[i], "<") == 0 || strcmp(test->input_splitted[i], ">") == 0 ||
                strcmp(test->input_splitted[i], "<<") == 0 || strcmp(test->input_splitted[i], ">>") == 0)
                count++;
        }
        i++;
    }
    return count;
}

void dir(t_shell *test)
{
    int j = 0;
    int y = 0;
    test->dir_count = 0;

    test->dir = malloc(sizeof(char **) * (test->count_pipe + 2));
    if (!test->dir)
        return;
    while (y < test->count_pipe + 1)
    {
        int x = 0;
        test->dir[y] = malloc(sizeof(char *) * (count_max_dir(test, y) + 1));
        if (!test->dir[y])
            return;
        while (test->input_splitted[j] && (strcmp(test->input_splitted[j], "|") != 0))
        {
            if (strcmp(test->input_splitted[j], "<") == 0 || strcmp(test->input_splitted[j], ">") == 0 ||
               strcmp(test->input_splitted[j], "<<") == 0 || strcmp(test->input_splitted[j], ">>") == 0)
            {
                if (test->input_splitted[j + 1])
                {
                    test->dir[y][x] = test->input_splitted[j + 1];
                    test->dir_count++;
                    x++;
                    j++;
                }
            }
            j++; 
        }
        test->dir[y][x] = NULL;
        if (test->input_splitted[j])
            j++;
        y++;
    }
    test->dir[y] = NULL;
}

//----------------------------------------------------------------------commands ==> command_count, ***command
int count_max_commands(t_shell *test)
{
    int count = 0;
    int i = 0;

    if (!test->input_splitted)
        return 0;
    while (test->input_splitted[i])
    {
        if (strcmp(test->input_splitted[i], "<<") != 0 &&
            strcmp(test->input_splitted[i], "<") != 0 &&
            strcmp(test->input_splitted[i], ">>") != 0 &&
            strcmp(test->input_splitted[i], ">") != 0 &&
            strcmp(test->input_splitted[i], "|") != 0 &&
            (i == 0 || (strcmp(test->input_splitted[i - 1], "<<") != 0 &&
                        strcmp(test->input_splitted[i - 1], "<") != 0 &&
                        strcmp(test->input_splitted[i - 1], ">>") != 0 &&
                        strcmp(test->input_splitted[i - 1], ">") != 0)))
            count++;
        i++;
    }
    return (count);
}
void command_count(t_shell *test)
{
    int count = 0;
    int i = 0;
    int j = 0;

    test->command = malloc(sizeof(char **) * (count_max_commands(test) + 1));
    while (test->input_splitted[i])
    {
        if (strcmp(test->input_splitted[i], "<<") != 0 &&
            strcmp(test->input_splitted[i], "<") != 0 &&
            strcmp(test->input_splitted[i], ">>") != 0 &&
            strcmp(test->input_splitted[i], ">") != 0 &&
            strcmp(test->input_splitted[i], "|") != 0 &&
            (i == 0 || (strcmp(test->input_splitted[i - 1], "<<") != 0 &&
                        strcmp(test->input_splitted[i - 1], "<") != 0 &&
                        strcmp(test->input_splitted[i - 1], ">>") != 0 &&
                        strcmp(test->input_splitted[i - 1], ">") != 0)))
        {
            test->command[j] = test->split_the_split[i];               
            j++;
            count++;
        }
        i++;
    }
    test->command[j] = NULL;
    test->command_count = count;
}
//------------------------------------------------------------------------
void count_pipe(t_shell *shell)
{
    int i = 0;
    while (shell->input[i])
    {
        if (shell->input[i] == '|')
            shell->count_pipe++;
        i++;
    }
}
