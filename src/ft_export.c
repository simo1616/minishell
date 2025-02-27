/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 08:54:23 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/05 08:54:23 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_export_name(char *args)
{
	int	i;

	if (!args || !args[0] || args[0] == '=' || ft_isdigit(args[0]))
		return (0);
	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	**ft_copy_env(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (i-- > 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

static void	ft_sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	ft_print_sorted_env(char **env)
{
	int		i;
	int		j;
	char	**sorted_env;

	sorted_env = ft_copy_env(env);
	if (!sorted_env)
		return ;
	ft_sort_env(sorted_env);
	i = 0;
	while (sorted_env[i])
	{
		ft_printf("declare -x ");
		j = 0;
		while (sorted_env[i][j] && sorted_env[i][j] != '=')
			ft_printf("%c", sorted_env[i][j++]);
		if (sorted_env[i][j] && sorted_env[i][j] == '=')
			ft_printf("=\"%s\"\n", &sorted_env[i][j + 1]);
		else
			ft_printf("\n");
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

int	ft_export(char **args, t_shell_env *shell_env)
{
	int		i;
	char	**new_env;

	if (!args || !args[0] || !shell_env)
		return (1);
	if (!args[1])
	{
		ft_print_sorted_env(shell_env->env);
		return (0);
	}
	if (!check_export_name(args[1]))
	{
		ft_printf("minishell: export: `%s': not a valid identifier\n", args[1]);
		return (1);
	}
	i = 0;
	while (shell_env->env[i])
	{
		if (ft_strncmp(shell_env->env[i], args[1], ft_strchr(args[1], '=') - args[1]) == 0)
		{
			free(shell_env->env[i]);
			shell_env->env[i] = ft_strdup(args[1]);
			return (0);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (shell_env->env[i])
	{
		new_env[i] = shell_env->env[i];
		i++;
	}
	new_env[i] = ft_strdup(args[1]);
	new_env[i + 1] = NULL;
	free(shell_env->env);
	shell_env->env = new_env;
	return (0);
}
