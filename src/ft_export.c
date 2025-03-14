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

static int	update_existing_var(char **env, char *arg)
{
	int	i;
	int	len;

	len = ft_strchr(arg, '=') - arg;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], arg, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(arg);
			if (!env[i])
				return (-1);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	add_new_var(t_shell_env *shell_env, char *arg)
{
	int		i;
	char	**new_env;

	i = 0;
	while (shell_env->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (shell_env->env[i])
	{
		new_env[i] = shell_env->env[i];
		i++;
	}
	new_env[i] = ft_strdup(arg);
	if (!new_env[i])
		return (free(new_env), 1);
	new_env[i + 1] = NULL;
	free(shell_env->env);
	shell_env->env = new_env;
	return (0);
}

int	ft_export(char **args, t_shell_env *shell_env)
{
	int	ret;

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
	ret = update_existing_var(shell_env->env, args[1]);
	if (ret == -1)
		return (1);
	if (ret == 1)
		return (0);
	return (add_new_var(shell_env, args[1]));
}
