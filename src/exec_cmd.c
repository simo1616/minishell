/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/12 10:01:13 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_cmd *cmds, t_shell_env *env)
{
	t_cmd		*current;
	t_pipe_info	pipes;
	int			saved_stdin;
	int			saved_stdout;

	pipes.prev_fd = -1;
	current = cmds;
	while (current)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (handle_redirections(current) == -1)
		{
			env->exit_status = 1;
			reset_std(saved_stdin, saved_stdout);
			return ;
		}
		if (!current->next && is_builtin(env, current->av[0]))
			env->exit_status = exec_builtin(current, env);
		else
			pipes.prev_fd = fork_and_exec(current, env, &pipes);
		reset_std(saved_stdin, saved_stdout);
		current = current->next;
	}
	close_prev_fd(pipes.prev_fd);
}
