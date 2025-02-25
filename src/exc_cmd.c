/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/25 11:12:49 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_cmd *cmds, t_shell_env *env)
{
	t_cmd	*current;
	int		ret;

	current = cmds;
	while (current)
	{
		handle_redirections(current);
		if (current->av && current->av[0])
		{
			if (is_builtin(env, current->av[0]))
			{
				ret = exec_builtin(current, env);
				env->exit_status = ret;
			}
			else
			{
				ret = excec_external(current, env);
				env->exit_status = ret;
			}
		}
		current = current->next;
	}
}
