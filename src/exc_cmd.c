/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/25 15:15:30 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_cmd *cmds, t_shell_env *env)
{
	t_cmd	*current;
	int		ret;
	int		saved_stdin;
	int		saved_stdout;

	current = cmds;
	while (current)
	{
		if (current->av && current->av[0])
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			handle_redirections(current);
			if (is_builtin(env, current->av[0]))
				ret = exec_builtin(current, env);
			else
				ret = excec_external(current, env);
			env->exit_status = ret;
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
		}
		current = current->next;
	}
}