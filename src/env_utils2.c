/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:05:00 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 13:05:43 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_env(t_shell_env *shell_env, char *new_var)
{
	char	**new_env;
	int		cnt;
	int		i;

	cnt = 0;
	while (shell_env->env[cnt])
		cnt++;
	new_env = malloc(sizeof(char *) * (cnt + 2));
	if (!new_env)
	{
		free(new_var);
		return (1);
	}
	i = 0;
	while (i < cnt)
	{
		new_env[i] = shell_env->env[i];
		i++;
	}
	new_env[cnt] = new_var;
	new_env[cnt + 1] = NULL;
	free(shell_env->env);
	shell_env->env = new_env;
	return (0);
}

int	env_set(t_shell_env *shell_env, const char *name, const char *value)
{
	char	*new_var;
	int		index;

	new_var = ft_strjoin_three(name, "=", value);
	if (!new_var)
		return (1);
	index = find_env_index(shell_env, name);
	if (index != -1)
	{
		free(shell_env->env[index]);
		shell_env->env[index] = new_var;
		return (0);
	}
	return (expand_env(shell_env, new_var));
}
