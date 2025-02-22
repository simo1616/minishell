/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excec_built.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:48:06 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 18:48:54 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_shell_env *env, char *cmd_name)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtins(env);
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, cmd_name) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_shell_env *env)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtins(env);
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, cmd->av[0]) == 0)
			return (builtins[i].func(cmd->av, env));
		i++;
	}
	return (0);
}
