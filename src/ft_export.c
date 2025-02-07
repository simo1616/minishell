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
			return (NULL);
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
	char	**sorted_env;

	sorted_env = ft_copy_env(env);
	if (!sorted_env)
		return ;
	ft_sort_env(sorted_env);
	i = 0;
	while (sorted_env[i])
	{
		ft_printf("declare -x %s\n", sorted_env[i]);
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
	if (!ft_strchr(args[1], '=') || args[1][0] == '=')
	{
		ft_printf("-minishell: export:'%s': not a valid identifier\n", args[1]);
		return (1);
	}
	i = 0;
	while (shell_env->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (shell_env->env[i])
	{
		new_env[i] = ft_strdup(shell_env->env[i]);
		free(shell_env->env[i]);
		i++;
	}
	new_env[i] = ft_strdup(args[1]);
	new_env[i + 1] = NULL;
	free(shell_env->env);
	shell_env->env = new_env;
	return (0);
}
