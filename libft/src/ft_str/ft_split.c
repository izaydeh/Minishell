/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shoaib <shoaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:17:20 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/06 21:28:10 by shoaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int is_redirection_operator(const char *op) {
    return (strcmp(op, ">") == 0 || strcmp(op, ">>") == 0 ||
            strcmp(op, "<") == 0 || strcmp(op, "<<") == 0);
}


void trim(char *str, char **start, char **end) {
    while (isspace((unsigned char)*str))
        str++;
    *start = str;
    if (*str == '\0') {
        *end = str;
        return;
    }
    char *last = str + strlen(str) - 1;
    while (last >= str && isspace((unsigned char)*last))
        last--;
    *end = last + 1;
}

int is_operator(const char *s, int *op_len) {
    if (s[0] == '>' && s[1] == '>') {
        *op_len = 2;
        return 1;
    } else if (s[0] == '<' && s[1] == '<') {
        *op_len = 2;
        return 1;
    } else if (s[0] == '>') {
        *op_len = 1;
        return 1;
    } else if (s[0] == '<') {
        *op_len = 1;
        return 1;
    } else if (s[0] == '|') {
        *op_len = 1;
        return 1;
    }
    *op_len = 0;
    return 0;
}


int tokenize_input(const char *input, char **tokens, int max_tokens, int *cmd_flags)
{
    int token_count = 0;
    int last_cmd_index = -1;   
    int has_command = 0;   
    const char *p = input;
    int prev_redir = 0;        

    while (*p != '\0' && token_count < max_tokens) {
       
        while (*p && isspace((unsigned char)*p))
            p++;
        if (*p == '\0')
            break;

        int op_len = 0;
        if (is_operator(p, &op_len))
        {
            char *op_token = malloc(op_len + 1);
            if (!op_token) { perror("malloc"); exit(EXIT_FAILURE); }
            strncpy(op_token, p, op_len);
            op_token[op_len] = '\0';
            tokens[token_count] = op_token;
            cmd_flags[token_count] = -1;
            token_count++;

          
            if (strcmp(op_token, "|") == 0) {
                has_command = 0;
                last_cmd_index = -1;
            }
        
            if (is_redirection_operator(op_token))
                prev_redir = 1;
            else
                prev_redir = 0;
            p += op_len;
        } else
        {
            const char *start_ptr = p;
            while (*p != '\0') {
                int len = 0;
                if (is_operator(p, &len))
                    break;
                p++;
            }
            int part_len = p - start_ptr;
            char *part = malloc(part_len + 1);
            if (!part) { perror("malloc"); exit(EXIT_FAILURE); }
            strncpy(part, start_ptr, part_len);
            part[part_len] = '\0';

            char *trim_start, *trim_end;
            trim(part, &trim_start, &trim_end);
            int trimmed_len = trim_end - trim_start;
            if (trimmed_len <= 0) {
                free(part);
                prev_redir = 0;
                continue;
            }

            if (prev_redir) {
                
                char *space = NULL;
                for (int i = 0; i < trimmed_len; i++) {
                    if (isspace((unsigned char)trim_start[i])) {
                        space = trim_start + i;
                        break;
                    }
                }
                if (space) {
                    int word_len = space - trim_start;
                    char *operand = malloc(word_len + 1);
                    if (!operand) { perror("malloc"); exit(EXIT_FAILURE); }
                    strncpy(operand, trim_start, word_len);
                    operand[word_len] = '\0';
                    tokens[token_count++] = operand;
                    cmd_flags[token_count - 1] = 0;  
                    char *rem = space;
                    while (rem < trim_end && isspace((unsigned char)*rem))
                        rem++;
                    if (rem < trim_end) {
                        int extra_len = trim_end - rem;
                        if (has_command && last_cmd_index != -1 && cmd_flags[last_cmd_index] == 1) {
                            size_t old_len = strlen(tokens[last_cmd_index]);
                            char *merged = malloc(old_len + 1 + extra_len + 1);
                            if (!merged) { perror("malloc"); exit(EXIT_FAILURE); }
                            sprintf(merged, "%s %.*s", tokens[last_cmd_index], extra_len, rem);
                            free(tokens[last_cmd_index]);
                            tokens[last_cmd_index] = merged;
                        } else {
                            char *extra = malloc(extra_len + 1);
                            if (!extra) { perror("malloc"); exit(EXIT_FAILURE); }
                            strncpy(extra, rem, extra_len);
                            extra[extra_len] = '\0';
                            tokens[token_count++] = extra;
                            cmd_flags[token_count - 1] = 0;
                            has_command = 1;
                            last_cmd_index = token_count - 1;
                        }
                    }
                } else {
                 
                    char *operand = malloc(trimmed_len + 1);
                    if (!operand) { perror("malloc"); exit(EXIT_FAILURE); }
                    strncpy(operand, trim_start, trimmed_len);
                    operand[trimmed_len] = '\0';
                    tokens[token_count++] = operand;
                    cmd_flags[token_count - 1] = 0;
                }
                prev_redir = 0;
            } else {
            
                char *token_str = malloc(trimmed_len + 1);
                if (!token_str) { perror("malloc"); exit(EXIT_FAILURE); }
                strncpy(token_str, trim_start, trimmed_len);
                token_str[trimmed_len] = '\0';
                tokens[token_count++] = token_str;
                cmd_flags[token_count - 1] = 1;
                has_command = 1;
                last_cmd_index = token_count - 1;
            }
            free(part);
        }
    }
    return token_count;
}

char **ft_split(const char *input) {
    const int max_tokens = 100;
    char **tokens = malloc((max_tokens + 1) * sizeof(char *));
    if (!tokens) { perror("malloc"); exit(EXIT_FAILURE); }
    int *cmd_flags = malloc(max_tokens * sizeof(int));
    if (!cmd_flags) { perror("malloc"); exit(EXIT_FAILURE); }

    int token_count = tokenize_input(input, tokens, max_tokens, cmd_flags);
    tokens[token_count] = NULL;
    free(cmd_flags);
    return tokens;
}

static size_t ft_count_subwords(char *s)
{
    size_t count = 0;

    while (*s)
    {
        while (*s == ' ')
            s++;
        if (*s)
        {
            count++;
            if (*s == '"')
            {
                s++;
                while (*s && *s != '"')
                    s++;
                if (*s == '"')
                    s++;
            }
            else
            {
                while (*s && *s != ' ')
                    s++;
            }
        }
    }
    return count;
}

static char **ft_split_whitespace(char *s)
{
    char **tokens;
    size_t j = 0;
    char *token;

    if (!s)
        return NULL;
    
    tokens = malloc((ft_count_subwords(s) + 1) * sizeof(char *));
    if (!tokens)
        return NULL;
    
    while (*s)
    {
        while (*s == ' ')
            s++;
        if (*s)
        {
            const char *start = s;
            if (*s == '"')
            {
                start = ++s;
                while (*s && *s != '"')
                    s++;
                token = ft_substr(start, 0, s - start);
                if (*s == '"')
                    s++;
            }
            else
            {
                while (*s && *s != ' ')
                    s++;
                token = ft_substr(start, 0, s - start);
            }
            tokens[j++] = token;
        }
    }
    tokens[j] = NULL;
    return tokens;
}

static char	**ft_split_echo(char *s)
{
	char	**tokens;
	const char	*start;
	
	tokens = malloc(4 * sizeof(char *));
	if (!tokens)
		return (NULL);
	while (*s == ' ')
		s++;
	start = s;
	while (*s && *s != ' ')
		s++;
	tokens[0] = strndup(start, s - start);
	while (*s == ' ')
		s++;
	if (*s == '-')
	{
		start = s;
		while (*s && *s != ' ')
			s++;
		tokens[1] = strndup(start, s - start);
		while (*s == ' ')
			s++;
		tokens[2] = strdup(s);
	}
	else
	{
		tokens[1] = strdup(s);
		tokens[2] = NULL;
	}
	tokens[3] = NULL;
	return tokens;
}


char ***split_commands(char **s)
{
    char ***final_split;
    size_t count = 0;

    if (!s)
        return NULL;
    while (s[count])
        count++;
    final_split = malloc((count + 1) * sizeof(char **));
    if (!final_split)
        return NULL;
    for (size_t i = 0; i < count; i++)
    {
        if (!ft_strncmp(s[i], "echo", 4) && (s[i][4] == ' ' || s[i][4] == '\0'))
            final_split[i] = ft_split_echo(s[i]);
        else
            final_split[i] = ft_split_whitespace(s[i]);
    }
    final_split[count] = NULL;
    return final_split;
}


