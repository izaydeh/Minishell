/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 02:03:58 by marvin            #+#    #+#             */
/*   Updated: 2025/02/11 02:03:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_shell *shell, char *name)
{
    int name_len = ft_strlen(name);
    int i = 0;
    while (shell->env[i])
    {
        if (!ft_strncmp(shell->env[i], name, name_len) && shell->env[i][name_len] == '=')
            return (shell->env[i] + name_len + 1);
        i++;
    }
    return ("");
}

/*
 * expand_token:
 *
 * Processes a raw token (with quotes) and returns a new allocated string
 * where the following rules hold:
 *
 *   - In the NORMAL (unquoted) state, a '$' is followed by a variable name
 *     (letters, digits, underscores) and is expanded.
 *
 *   - In a DOUBLE-QUOTED segment, variables are expanded; the surrounding
 *     double quotes are removed.
 *
 *   - In a SINGLE-QUOTED segment, everything is taken literally and no
 *     expansion is performed; the surrounding single quotes are removed.
 *
 *   Adjacent quoted segments are concatenated just as bash does.
 */
char	*expand_token(const char *token, t_shell *shell)
{
    enum { NORMAL, SINGLE, DOUBLE } state = NORMAL;
    size_t i = 0, j = 0;
    size_t len = ft_strlen(token);
    /* Over–estimate the output buffer size (you may later realloc if needed) */
    size_t capacity = len * 2 + 1;
    char *result = malloc(capacity);
    if (!result)
        return (NULL);
    
    while (i < len)
    {
        char c = token[i];
        if (state == NORMAL)
        {
            if (c == '\'')
            {
                state = SINGLE;
                i++;  // Skip the opening single quote
            }
            else if (c == '"')
            {
                state = DOUBLE;
                i++;  // Skip the opening double quote
            }
            else if (c == '$')
            {
                i++;  // Skip '$'
                /* Check if the next character begins a valid variable name */
                if (i >= len || (!(ft_isalpha(token[i]) || token[i] == '_')))
                {
                    result[j++] = '$';
                }
                else
                {
                    size_t start = i;
                    while (i < len && (ft_isalnum(token[i]) || token[i] == '_'))
                        i++;
                    char *var_name = ft_substr(token, start, i - start);
                    if (!var_name)
                    {
                        free(result);
                        return (NULL);
                    }
                    char *value = get_env_value(shell, var_name);
                    free(var_name);
                    for (size_t k = 0; value[k] != '\0'; k++)
                        result[j++] = value[k];
                }
            }
            else
            {
                result[j++] = c;
                i++;
            }
        }
        else if (state == SINGLE)
        {
            if (c == '\'')
            {
                state = NORMAL;
                i++;  // Skip the closing single quote
            }
            else
            {
                result[j++] = c;
                i++;
            }
        }
        else if (state == DOUBLE)
        {
            if (c == '"')
            {
                state = NORMAL;
                i++;  // Skip the closing double quote
            }
            else if (c == '$')
            {
                i++;  // Skip '$'
                if (i >= len || (!(ft_isalpha(token[i]) || token[i] == '_')))
                {
                    result[j++] = '$';
                }
                else
                {
                    size_t start = i;
                    while (i < len && (ft_isalnum(token[i]) || token[i] == '_'))
                        i++;
                    char *var_name = ft_substr(token, start, i - start);
                    if (!var_name)
                    {
                        free(result);
                        return (NULL);
                    }
                    char *value = get_env_value(shell, var_name);
                    free(var_name);
                    for (size_t k = 0; value[k] != '\0'; k++)
                        result[j++] = value[k];
                }
            }
            else
            {
                result[j++] = c;
                i++;
            }
        }
    }
    result[j] = '\0';
    return (result);
}