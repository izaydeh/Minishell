/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sal-kawa <sal-kawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:17:20 by sal-kawa          #+#    #+#             */
/*   Updated: 2025/02/05 15:14:48 by sal-kawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_is_special_char(const char *s, size_t *length)
{
    if (!s || !*s)
        return (0);
    if ((s[0] == '<' && s[1] == '<') || (s[0] == '>' && s[1] == '>'))
    {
        *length = 2;
        return (1);
    }
    if (s[0] == '|' || s[0] == '<' || s[0] == '>')
    {
        *length = 1;
        return (1);
    }
    *length = 0;
    return (0);
}

static size_t ft_countword(const char *s)
{
    size_t count = 0;
    int in_quote = 0;
    size_t special_len;

    while (*s)
    {
        while (*s == ' ' && !in_quote)
            s++;
        if (*s)
        {
            count++;
            if (!in_quote && ft_is_special_char(s, &special_len))
                s += special_len;
            else
            {
                while (*s && (in_quote || (!ft_is_special_char(s, &special_len) && *s != ' ')))
                {
                    if (*s == '"')
                        in_quote = !in_quote;
                    s++;
                }
            }
        }
    }
    return (count);
}

static char *ft_get_next_token(const char **s, int *in_quote)
{
    const char *start;
    size_t special_len;
    char *token;
    size_t len;

    while (**s == ' ' && !*in_quote)
        (*s)++;
    start = *s;
    if (!*in_quote && ft_is_special_char(*s, &special_len))
    {
        *s += special_len;
        return (ft_substr(start, 0, special_len));
    }
    while (**s && (*in_quote || !ft_is_special_char(*s, &special_len)))
    {
        if (**s == '"')
            *in_quote = !*in_quote;
        (*s)++;
    }
    token = ft_substr(start, 0, *s - start);
    if (!*in_quote && token)
    {
        len = ft_strlen(token);
        while (len > 0 && token[len - 1] == ' ')
            len--;
        char *trimmed = ft_substr(token, 0, len);
        free(token);
        token = trimmed;
    }
    return (token);
}

char **ft_split(const char *s)
{
    char **tokens;
    size_t j = 0;
    int in_quote = 0;
    char *token;

    if (!s)
        return (NULL);
    tokens = malloc((ft_countword(s) + 1) * sizeof(char *));
    if (!tokens)
        return (NULL);
    while (*s)
    {
        token = ft_get_next_token(&s, &in_quote);
        if (token && *token)
            tokens[j++] = token;
        else
            free(token);
    }
    tokens[j] = NULL;
    return (tokens);
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

