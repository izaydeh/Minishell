#include "minishell.h"

/*
 * Helper: Check if a string (up to an optional '=' sign)
 * is a valid identifier. (Identifier must start with a letter or '_' and
 * then may contain letters, digits or '_'.)
 */
static int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/*
 * Helper: Extract the name part from a variable string.
 * For example, given "FOO=bar" it returns a newly allocated "FOO".
 */
static char	*get_name(const char *str)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = malloc(i + 1);
	if (!name)
		return (NULL);
	for (int j = 0; j < i; j++)
		name[j] = str[j];
	name[i] = '\0';
	return (name);
}

/*
 * The export function.
 *
 * - If no extra arguments are given (i.e. args[1] == NULL), the function
 *   prints all environment variables in sorted order.
 *
 * - If arguments are provided:
 *     - It first checks if the argument is a valid identifier.
 *    
	- Then it looks for an '=' character. If none is found (e.g. "export var"),
 *       the argument is ignored (nothing is added to the environment).
 *     - Otherwise (e.g. "export var=" or "export var=value"),
	it adds or updates
 *       the variable in the environment.
 */
void	ft_export(t_shell *test, char **args)
{
	int		i;
	char	*equal_sign;
	char	*new_var;
	char	*var_name;
	char **env_copy;
	char	*temp;
	int		name_len;
	int		j;
	int		found;
	char	*env_name;
	int		count;
	char **new_env;

	/* No extra arguments: print environment in sorted order */
	if (!args[1])
	{
		count = 0;
		int j, k;
		while (test->env[count])
			count++;
		env_copy = malloc((count + 1) * sizeof(char *));
		if (!env_copy)
			return ;
		/* Copy pointers (we won’t modify the strings) */
		for (j = 0; j < count; j++)
			env_copy[j] = test->env[j];
		env_copy[count] = NULL;
		/* Simple bubble sort */
		for (j = 0; j < count - 1; j++)
		{
			for (k = j + 1; k < count; k++)
			{
				if (ft_strcmp(env_copy[j], env_copy[k]) > 0)
				{
					temp = env_copy[j];
					env_copy[j] = env_copy[k];
					env_copy[k] = temp;
				}
			}
		}
		/* Print in the format: declare -x NAME="value" */
		for (j = 0; j < count; j++)
		{
			write(1, "declare -x ", 11);
			equal_sign = ft_strchr(env_copy[j], '=');
			if (equal_sign)
			{
				name_len = equal_sign - env_copy[j];
				write(1, env_copy[j], name_len);
				write(1, "=\"", 2);
				write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
				write(1, "\"\n", 2);
			}
			else
			{
				write(1, env_copy[j], ft_strlen(env_copy[j]));
				write(1, "\n", 1);
			}
		}
		free(env_copy);
	}
	else
	{
		/* Process each argument after "export" */
		for (i = 1; args[i]; i++)
		{
			/* Validate identifier */
			if (!is_valid_identifier(args[i]))
			{
				write(2, "export: `", 9);
				write(2, args[i], ft_strlen(args[i]));
				write(2, "': not a valid identifier\n", 27);
				continue ;
			}
			/* Check for '=' character */
			equal_sign = ft_strchr(args[i], '=');
			if (!equal_sign)
			{
				/* If no '=' is found (e.g. "export var"), do nothing */
				continue ;
			}
			/* For arguments with '=', we use the argument as is */
			new_var = args[i];
			/* Get the variable name (part before '=') */
			var_name = get_name(new_var);
			{
				j = 0;
				found = 0;
				/* Search if the variable already exists in test->env */
				while (test->env[j])
				{
					env_name = get_name(test->env[j]);
					if (ft_strcmp(var_name, env_name) == 0)
					{
						found = 1;
						/* Replace the old variable */
						free(test->env[j]);
						test->env[j] = ft_strdup(new_var);
						free(env_name);
						break ;
					}
					free(env_name);
					j++;
				}
				/* If variable was not found, add it to the environment */
				if (!found)
				{
					count = 0;
					while (test->env[count])
						count++;
					new_env = malloc((count + 2) * sizeof(char *));
					if (!new_env)
						return ;
					for (j = 0; j < count; j++)
						new_env[j] = test->env[j];
					new_env[count] = ft_strdup(new_var);
					new_env[count + 1] = NULL;
					free(test->env);
					test->env = new_env;
				}
			}
			free(var_name);
		}
	}
}
