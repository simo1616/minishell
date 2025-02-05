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

int	ft_export(char **args, t_shell_env *shell_env)
{
	int		i;
	char	**new_env;

	if (!args || !args[0] || !shell_env)
		return (1);
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



