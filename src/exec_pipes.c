/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:25:52 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/11 15:15:59 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe(t_pipe_info *pipes, t_cmd *cur)
{
	if (cur->next && pipe(pipes->fd) < 0)
	{
		perror("error pipe");
		return (-1);
	}
	return (0);
}

int	exec_pipes(t_cmd *cmds, t_shell_env *env)
{
	t_pipe_info	pipes;
	t_cmd		*cur;
	int			i;
	int			status;

	if (init_pipe_info(&pipes, cmds) == -1)
		return (EXIT_FAILURE);
	cur = cmds;
	i = 0;
	while (cur)
	{
		if (setup_pipe(&pipes, cur) == -1)
			return (free(pipes.pids), EXIT_FAILURE);
		pipes.pids[i] = fork();
		if (pipes.pids[i] < 0)
			return (perror("error fork"), free(pipes.pids), EXIT_FAILURE);
		if (pipes.pids[i] == 0)
			child_process_pipes(cur, env, &pipes);
		parent_process_pipes(&pipes, cur);
		cur = cur->next;
		i++;
	}
	wait_for_children(pipes.pids, i, &status, env);
	return (free(pipes.pids), env->exit_status);
}
