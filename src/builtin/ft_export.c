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
	i = 0;
	while (str[i] && str[i] != '=')
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

static char	*get_invalid_identifier(const char *str)
{
	int	i;

	if (!str)
		return (ft_strdup(""));
	if (str[0] != '=')
		return (ft_strdup(str));
	i = 1;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (ft_substr(str, 0, i + 1));
}

static int	count_array(char **arr)
{
	int	count;

	count = 0;
	while (arr && arr[count])
		count++;
	return (count);
}

static char	**merge_lists(t_shell *shell)
{
	int		count_env;
	int		count_exp;
	int		total;
	char	**merged;
	int		i;
	int		j;

	count_env = count_array(shell->env);
	count_exp = count_array(shell->exp);
	total = count_env + count_exp;
	merged = malloc((total + 1) * sizeof(char *));
	if (!merged)
		return (NULL);
	j = 0;
	i = 0;
	while (i < count_env)
	{
		merged[j++] = ft_strdup(shell->env[i]);
		i++;
	}
	i = 0;
	while (i < count_exp)
	{
		merged[j++] = ft_strdup(shell->exp[i]);
		i++;
	}
	merged[j] = NULL;
	return (merged);
}

static void	sort_array(char **arr, int total)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < total)
	{
		j = i + 1;
		while (j < total)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_array(char **arr)
{
	int		i;
	char	*eq;
	int		len;

	i = 0;
	while (arr[i])
	{
		write(1, "declare -x ", 11);
		eq = ft_strchr(arr[i], '=');
		if (eq)
		{
			len = eq - arr[i];
			write(1, arr[i], len);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, arr[i], ft_strlen(arr[i]));
			write(1, "\n", 1);
		}
		i++;
	}
}

static void	print_export_list(t_shell *shell)
{
	char	**merged;
	int		total;
	int		i;

	merged = merge_lists(shell);
	if (!merged)
		return ;
	total = count_array(merged);
	sort_array(merged, total);
	print_array(merged);
	i = 0;
	while (merged[i])
	{
		free(merged[i]);
		i++;
	}
	free(merged);
}

static void	update_env_var(t_shell *shell, char *var_name, char *new_var)
{
	int		i;
	int		found;
	char	*env_name;
	int		cnt;
	char	**new_env;
	int		j;

	i = 0;
	found = 0;
	while (shell->env && shell->env[i])
	{
		env_name = get_name(shell->env[i]);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			found = 1;
			free(shell->env[i]);
			shell->env[i] = ft_strdup(new_var);
			free(env_name);
			break ;
		}
		free(env_name);
		i++;
	}
	if (!found)
	{
		cnt = count_array(shell->env);
		new_env = malloc((cnt + 2) * sizeof(char *));
		if (!new_env)
			return ;
		j = 0;
		while (j < cnt)
		{
			new_env[j] = shell->env[j];
			j++;
		}
		new_env[cnt] = ft_strdup(new_var);
		new_env[cnt + 1] = NULL;
		free(shell->env);
		shell->env = new_env;
	}
}

static void	remove_from_export(t_shell *shell, char *var_name)
{
	int	i;
	int	cnt;
	int	pos;

	cnt = count_array(shell->exp);
	i = 0;
	while (i < cnt)
	{
		if (ft_strcmp(var_name, shell->exp[i]) == 0)
		{
			pos = i;
			while (pos < cnt - 1)
			{
				shell->exp[pos] = shell->exp[pos + 1];
				pos++;
			}
			shell->exp[cnt - 1] = NULL;
			break ;
		}
		i++;
	}
}

static void	add_to_export(t_shell *shell, char *var)
{
	int	exists;
	int	i;
	int	cnt;
	char	**new_exp;
	int	j;

	exists = 0;
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strcmp(var, get_name(shell->env[i])) == 0)
			exists = 1;
		i++;
	}
	i = 0;
	while (shell->exp && shell->exp[i])
	{
		if (ft_strcmp(var, shell->exp[i]) == 0)
			exists = 1;
		i++;
	}
	if (!exists)
	{
		cnt = count_array(shell->exp);
		new_exp = malloc((cnt + 2) * sizeof(char *));
		if (!new_exp)
			return ;
		j = 0;
		while (j < cnt)
		{
			new_exp[j] = shell->exp[j];
			j++;
		}
		new_exp[cnt] = ft_strdup(var);
		new_exp[cnt + 1] = NULL;
		free(shell->exp);
		shell->exp = new_exp;
	}
}

static void	process_arg(t_shell *shell, char *arg, int *valid_args, int *error_flag)
{
	char	*local_arg;
	char	*arg_exp;
	char	*eq;
	char	*var_name;
	char	*new_var;

	local_arg = arg;
	arg_exp = NULL;
	if (arg[0] == '$')
	{
		if (ft_strchr(arg, '='))
		{
			arg_exp = expander(arg, shell);
			if (!arg_exp || arg_exp[0] == '=')
			{
				write(2, "export: `", 9);
				write(2, (arg_exp ? arg_exp : ""), ft_strlen(arg_exp ? arg_exp : ""));
				write(2, "': not a valid identifier\n", 27);
				shell->exit_status = 1;
				*error_flag = 1;
				if (arg_exp)
					free(arg_exp);
				return ;
			}
			local_arg = arg_exp;
		}
		else
		{
			arg_exp = expander(arg, shell);
			if (!arg_exp || arg_exp[0] == '\0')
			{
				if (arg_exp)
					free(arg_exp);
				return ;
			}
			local_arg = arg_exp;
		}
	}
	if (!is_valid_identifier(local_arg))
	{
		char *inv = get_invalid_identifier(local_arg);
		write(2, "export: `", 9);
		write(2, inv, ft_strlen(inv));
		write(2, "': not a valid identifier\n", 27);
		shell->exit_status = 1;
		*error_flag = 1;
		free(inv);
		if (arg_exp)
		{
			free(arg_exp);
			arg_exp = NULL;
		}
		return ;
	}
	(*valid_args)++;
	eq = ft_strchr(local_arg, '=');
	if (!eq)
	{
		add_to_export(shell, local_arg);
	}
	else
	{
		if (*(eq + 1) == '$')
		{
			char *exp_val = expander(eq + 1, shell);
			if (!exp_val || exp_val[0] == '=')
			{
				write(2, "export: `", 9);
				write(2, "=", 1);
				write(2, (exp_val ? exp_val : ""), ft_strlen(exp_val ? exp_val : ""));
				write(2, "': not a valid identifier\n", 27);
				shell->exit_status = 1;
				*error_flag = 1;
				if (exp_val)
					free(exp_val);
				if (arg_exp)
					free(arg_exp);
				return ;
			}
			{
				char *left = get_name(local_arg);
				char *temp = ft_strjoin(left, "=");
				new_var = ft_strjoin(temp, exp_val);
				free(left);
				free(temp);
				free(exp_val);
			}
		}
		else
			new_var = local_arg;
		var_name = get_name(new_var);
		update_env_var(shell, var_name, new_var);
		remove_from_export(shell, var_name);
		free(var_name);
	}
	if (arg_exp)
	{
		free(arg_exp);
		arg_exp = NULL;
	}
}

void	ft_export(t_shell *shell, char **args)
{
	int	valid_args;
	int	error_flag;
	int	i;

	valid_args = 0;
	error_flag = 0;
	if (!args[1])
	{
		print_export_list(shell);
		return ;
	}
	i = 1;
	while (args[i])
	{
		process_arg(shell, args[i], &valid_args, &error_flag);
		i++;
	}
	if (valid_args == 0 && error_flag == 0)
		print_export_list(shell);
}
