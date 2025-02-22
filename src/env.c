/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:59:17 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 13:00:09 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**dup_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (--i >= 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

t_shell_env	*create_shell_env(char **envp)
{
	t_shell_env	*shell_env;

	shell_env = malloc(sizeof(t_shell_env));
	if (!shell_env)
		return (NULL);
	shell_env->env = dup_env(envp);
	if (!shell_env->env)
	{
		free(shell_env);
		return (NULL);
	}
	shell_env->exit_status = 0;
	shell_env->running = 1;
	return (shell_env);
}

void	destroy_shell_env(t_shell_env *shell_env)
{
	int	i;

	if (!shell_env)
		return ;
	if (shell_env->env)
	{
		i = 0;
		while (shell_env->env[i])
			free(shell_env->env[i++]);
		free(shell_env->env);
	}
	free(shell_env);
}
