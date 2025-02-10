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
#include <ctype.h>o

/* === Helpers =================================================== */

// Return 1 if op is one of the redirection operators.
int is_redirection_operator(const char *op) {
    return (strcmp(op, ">") == 0 || strcmp(op, ">>") == 0 ||
            strcmp(op, "<") == 0 || strcmp(op, "<<") == 0);
}

// Trim leading and trailing whitespace from str.  
// The pointers *start and *end are set to the first non–space and one–past–the–last non–space character.
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

// Check if the string at s begins with an operator token.
// If yes, set *op_len to its length (1 or 2) and return 1; otherwise return 0.
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

/* === Tokenizer ===================================================

   The function below “tokenizes” the input string into tokens and
   stores them in the provided array tokens (with capacity max_tokens).
   
   In addition, an integer array cmd_flags (of the same length) is used
   to mark tokens that are created from a “normal” (non–redirection) chunk
   (flag 1) versus those that are produced as part of a redirection operand
   (flag 0). Operator tokens are marked with –1.
   
   The idea is that when a redirection operator is encountered the next
   non–operator chunk is split at its first space. Its first word is always
   output as a token (the “operand”). If there is extra text:
     • If the current command group already has a normal token (cmd_flags==1)
       then that extra text is merged (appended, with a separating space)
       into that token.
     • Otherwise, the extra text is output as a separate token and marked as
       a command token.
   
   A “command group” is delimited by the pipe operator (“|”). */

int tokenize_input(const char *input, char **tokens, int max_tokens, int *cmd_flags) {
    int token_count = 0;
    int last_cmd_index = -1;   // index of the last token that was created normally
    int has_command = 0;       // flag for current command group (set to 1 when a normal token is seen)
    const char *p = input;
    int prev_redir = 0;        // flag: next non–operator chunk is redirection operand

    while (*p != '\0' && token_count < max_tokens) {
        // Skip any leading whitespace.
        while (*p && isspace((unsigned char)*p))
            p++;
        if (*p == '\0')
            break;

        int op_len = 0;
        if (is_operator(p, &op_len)) {
            // Process an operator token.
            char *op_token = malloc(op_len + 1);
            if (!op_token) { perror("malloc"); exit(EXIT_FAILURE); }
            strncpy(op_token, p, op_len);
            op_token[op_len] = '\0';
            tokens[token_count] = op_token;
            cmd_flags[token_count] = -1;  // operator
            token_count++;

            // If the operator is a pipe, reset command–group info.
            if (strcmp(op_token, "|") == 0) {
                has_command = 0;
                last_cmd_index = -1;
            }
            // For redirection operators (<, <<, >, >>) set flag.
            if (is_redirection_operator(op_token))
                prev_redir = 1;
            else
                prev_redir = 0;
            p += op_len;
        } else {
            // Process a non–operator chunk.
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

            // Trim the part.
            char *trim_start, *trim_end;
            trim(part, &trim_start, &trim_end);
            int trimmed_len = trim_end - trim_start;
            if (trimmed_len <= 0) {
                free(part);
                prev_redir = 0;
                continue;
            }

            if (prev_redir) {
                /* Redirection operand: split at the first whitespace. */
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
                    cmd_flags[token_count - 1] = 0;  // redir operand (not a normal command token)

                    /* Now process extra text (if any) after the first word. */
                    char *rem = space;
                    while (*rem && isspace((unsigned char)*rem))
                        rem++;
                    if (*rem != '\0') {
                        /* If the current command group already has a normal token,
                           then merge the extra text into that token.
                           Otherwise, output the extra text as a new token and mark it as a command token. */
                        if (has_command && last_cmd_index != -1 && cmd_flags[last_cmd_index] == 1) {
                            size_t old_len = strlen(tokens[last_cmd_index]);
                            size_t extra_len = strlen(rem);
                            char *merged = malloc(old_len + 1 + extra_len + 1);
                            if (!merged) { perror("malloc"); exit(EXIT_FAILURE); }
                            sprintf(merged, "%s %s", tokens[last_cmd_index], rem);
                            free(tokens[last_cmd_index]);
                            tokens[last_cmd_index] = merged;
                        } else {
                            char *extra = strdup(rem);
                            if (!extra) { perror("strdup"); exit(EXIT_FAILURE); }
                            tokens[token_count++] = extra;
                            cmd_flags[token_count - 1] = 0;  // token from redir extra text
                            /* Now mark this as the command token for the current group. */
                            has_command = 1;
                            last_cmd_index = token_count - 1;
                        }
                    }
                } else {
                    /* No whitespace found: the whole trimmed part is the operand. */
                    char *operand = strdup(trim_start);
                    if (!operand) { perror("strdup"); exit(EXIT_FAILURE); }
                    tokens[token_count++] = operand;
                    cmd_flags[token_count - 1] = 0;
                }
                prev_redir = 0;
            } else {
                /* Normal (non–redirection) token: keep the whole trimmed string. */
                char *token_str = malloc(trimmed_len + 1);
                if (!token_str) { perror("malloc"); exit(EXIT_FAILURE); }
                strncpy(token_str, trim_start, trimmed_len);
                token_str[trimmed_len] = '\0';
                tokens[token_count++] = token_str;
                /* Mark this token as a normal command token. */
                cmd_flags[token_count - 1] = 1;
                has_command = 1;
                last_cmd_index = token_count - 1;
            }
            free(part);
        }
    }
    return token_count;
}

/* === ft_split ===================================================

   Allocates and returns a NULL–terminated array of tokens parsed from input.
   The caller must free each token and the array.
*/
char **ft_split(const char *input) {
    const int max_tokens = 100;
    char **tokens = malloc((max_tokens + 1) * sizeof(char *));
    if (!tokens) { perror("malloc"); exit(EXIT_FAILURE); }
    /* We use an auxiliary array for command–token flags.
       (Operators: –1, normal command tokens: 1, redirection–operand tokens: 0.) */
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
        final_split[i] = ft_split_whitespace(s[i]);
    final_split[count] = NULL;
    return final_split;
}

