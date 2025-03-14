/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:34:06 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/14 15:33:45 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_unset_name(char *args)
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

static int	check_unset_arg(char *arg)
{
	if (!check_unset_name(arg))
	{
		ft_printf("minishell: unset: `%s': not a valid identifier\n", arg);
		return (1);
	}
	return (0);
}

static int	env_var_index(char **env, char *var)
{
	int	i;
	int	len;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	remove_env_var(char **env, int index)
{
	free(env[index]);
	while (env[index])
	{
		env[index] = env[index + 1];
		index++;
	}
}

int	ft_unset(char **args, t_shell_env *shell_env)
{
	int	i;
	int	index;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (check_unset_arg(args[i]))
			return (1);
		index = env_var_index(shell_env->env, args[i]);
		if (index != -1)
			remove_env_var(shell_env->env, index);
		i++;
	}
	return (0);
}
