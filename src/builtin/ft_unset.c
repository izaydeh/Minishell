#include "minishell.h"

/*
 * Helper: Check if a string (up to an optional '=' sign)
 * is a valid identifier. (Identifier must start with a letter or '_'
 * and then may contain letters, digits or '_'.)
 */
static int	is_valid_identifier(const char *str)
{
	int i = 0;

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
 * Helper: Extract the name part from an environment string.
 * For example, given "FOO=bar", it returns a newly allocated "FOO".
 */
static char	*get_name(const char *str)
{
	int		i = 0;
	char	*name;

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
 * The unset function.
 *
 * For each argument after "unset":
 *   - Check if it is a valid identifier.
 *   - If valid, search the environment array for an entry whose name matches.
 *   - If found, free that entry and shift the remaining entries left.
 *
 * If no argument is provided (i.e. only "unset" is entered), do nothing.
 */
void	ft_unset(t_shell *test, char **args)
{
	int		i;
	int		j;
	int		k;
	char	*env_name;

	/* If no variable names are provided, nothing to do */
	if (!args[1])
		return;

	/* Process each variable name provided after the command "unset" */
	for (i = 1; args[i] != NULL; i++)
	{
		/* Validate the identifier */
		if (!is_valid_identifier(args[i]))
		{
			write(2, "unset: `", 8);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 27);
			continue;
		}
		j = 0;
		/* Loop through the environment array */
		while (test->env[j])
		{
			/* Extract the name from the current environment string */
			env_name = get_name(test->env[j]);
			if (!env_name)
				break;
			/* Compare the environment variable name with the argument */
			if (ft_strcmp(env_name, args[i]) == 0)
			{
				free(env_name);
				/* Found a match: free the environment string */
				free(test->env[j]);
				/* Shift subsequent entries left to fill the gap */
				k = j;
				while (test->env[k + 1])
				{
					test->env[k] = test->env[k + 1];
					k++;
				}
				/* Set the last pointer to NULL */
				test->env[k] = NULL;
				/*
				 * Do not increment j here as a new entry now resides at index j.
				 * (In most cases there is only one match per variable name.)
				 */
				continue;
			}
			free(env_name);
			j++;
		}
	}
}
