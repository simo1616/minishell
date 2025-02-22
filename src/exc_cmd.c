/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/22 17:11:53 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_cmd *cmds, t_shell_env *env)
{
	t_cmd	*current;
	t_redir	*redir;
	int		ret;

	current = cmds;
	while (current)
	{
		if (current->av && current->av[0])
		{
			if (is_builtin(current->av[0]))
			{
				ret = excec_builin(current, env);
				env->exit_status = ret;
			}
			else
			{
				ret = excec_external(current, env);
				env->exit_status = ret;
			}
		}
		redir = current->redirs;
		while (redir)
		{
			printf("\nRedirection: type=%d, file=%s\n", redir->type,
				redir->filename);
			redir = redir->next;
		}
		current = current->next;
	}
}
