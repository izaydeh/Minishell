#include "minishell.h"


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


void	ft_unset(t_shell *test, char **args)
{
	int		i;
	int		j;
	int		k;
	char	*env_name;

	if (!args[1])
		return ;
	for (i = 1; args[i] != NULL; i++)
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "unset: `", 8);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 27);
			continue ;
		}
		j = 0;
		while (test->env && test->env[j])
		{
			env_name = get_name(test->env[j]);
			if (!env_name)
				break ;
			if (ft_strcmp(env_name, args[i]) == 0)
			{
				free(env_name);
				free(test->env[j]);
				k = j;
				while (test->env[k + 1])
				{
					test->env[k] = test->env[k + 1];
					k++;
				}
				test->env[k] = NULL;
				continue ;
			}
			free(env_name);
			j++;
		}
		j = 0;
		while (test->exp && test->exp[j])
		{
			if (ft_strcmp(test->exp[j], args[i]) == 0)
			{
				free(test->exp[j]);
				k = j;
				while (test->exp[k + 1])
				{
					test->exp[k] = test->exp[k + 1];
					k++;
				}
				test->exp[k] = NULL;
				continue ;
			}
			j++;
		}
	}
}
