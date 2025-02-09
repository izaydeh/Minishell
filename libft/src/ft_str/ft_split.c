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

// Return 1 if op is one of the redirection operators.
int is_redirection_operator(const char *op) {
    return (strcmp(op, ">") == 0 || strcmp(op, ">>") == 0 ||
            strcmp(op, "<") == 0 || strcmp(op, "<<") == 0);
}

// Helper: Trim leading and trailing whitespace and set *start and *end pointers.
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

// Helper: Check if the string at s is an operator.
// If yes, set *op_len to the operator's length and return 1; otherwise, return 0.
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

// Tokenize the input string according to the rules described.
// Tokens are stored in the provided array 'tokens' (which should have capacity 'max_tokens').
// Returns the number of tokens stored.
int tokenize_input(const char *input, char **tokens, int max_tokens) {
    int token_count = 0;
    const char *p = input;
    int prev_redir = 0;  // Set to 1 if the previous token was a redirection operator.

    while (*p != '\0' && token_count < max_tokens) {
        // Skip any leading whitespace.
        while (*p && isspace((unsigned char)*p))
            p++;
        if (*p == '\0')
            break;

        int op_len = 0;
        if (is_operator(p, &op_len)) {
            // Process the operator token.
            char *op_token = malloc(op_len + 1);
            if (!op_token) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strncpy(op_token, p, op_len);
            op_token[op_len] = '\0';
            tokens[token_count++] = op_token;

            // Set flag if it is a redirection operator.
            if (is_redirection_operator(op_token))
                prev_redir = 1;
            else
                prev_redir = 0;
            p += op_len;
        } else {
            // Process a non-operator part.
            const char *start_ptr = p;
            while (*p != '\0') {
                int len = 0;
                if (is_operator(p, &len))
                    break;
                p++;
            }
            int part_len = p - start_ptr;
            char *part = malloc(part_len + 1);
            if (!part) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strncpy(part, start_ptr, part_len);
            part[part_len] = '\0';

            // Trim the part.
            char *trimmed_start, *trimmed_end;
            trim(part, &trimmed_start, &trimmed_end);
            int trimmed_len = trimmed_end - trimmed_start;
            if (trimmed_len <= 0) {
                free(part);
                prev_redir = 0;
                continue;
            }

            if (prev_redir) {
                // If the previous token was a redirection operator,
                // split the trimmed part at the first whitespace.
                char *space = strchr(trimmed_start, ' ');
                if (space) {
                    // First word.
                    int word_len = space - trimmed_start;
                    char *first_word = malloc(word_len + 1);
                    if (!first_word) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(EXIT_FAILURE);
                    }
                    strncpy(first_word, trimmed_start, word_len);
                    first_word[word_len] = '\0';
                    tokens[token_count++] = first_word;
                    if (token_count >= max_tokens) {
                        free(part);
                        break;
                    }
                    // Remainder (if any).
                    char *rem = space + 1;
                    while (*rem && isspace((unsigned char)*rem))
                        rem++;
                    if (*rem != '\0') {
                        char *second_word = strdup(rem);
                        if (!second_word) {
                            fprintf(stderr, "Memory allocation failed\n");
                            exit(EXIT_FAILURE);
                        }
                        tokens[token_count++] = second_word;
                    }
                } else {
                    // No space found; take the entire trimmed part.
                    char *token_str = strdup(trimmed_start);
                    if (!token_str) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(EXIT_FAILURE);
                    }
                    tokens[token_count++] = token_str;
                }
            } else {
                // Otherwise, take the entire trimmed part as one token.
                char *token_str = malloc(trimmed_len + 1);
                if (!token_str) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                strncpy(token_str, trimmed_start, trimmed_len);
                token_str[trimmed_len] = '\0';
                tokens[token_count++] = token_str;
            }
            free(part);
            prev_redir = 0; // Reset flag after processing a non-operator part.
        }
    }
    return token_count;
}

// ft_split: Accepts an input string, tokenizes it, and returns a dynamically allocated,
// NULL-terminated array of tokens. The caller must free each token and the array.
char **ft_split(const char *input) {
    const int max_tokens = 100;
    char **tokens = malloc((max_tokens + 1) * sizeof(char *));
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int token_count = tokenize_input(input, tokens, max_tokens);
    tokens[token_count] = NULL;
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

