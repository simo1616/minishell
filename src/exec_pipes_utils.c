/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:14:13 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/11 15:14:13 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_lstsize_cmd(t_cmd *cmds)
{
	int		size;
	t_cmd	*cur;

	size = 0;
	cur = cmds;
	while (cur)
	{
		cur = cur->next;
		size++;
	}
	return (size);
}

int	init_pipe_info(t_pipe_info *pipes, t_cmd *cmds)
{
	pipes->prev_fd = -1;
	pipes->cmd_count = ft_lstsize_cmd(cmds);
	pipes->pids = malloc(sizeof(pid_t) * pipes->cmd_count);
	if (!pipes->pids)
	{
		perror("error PID allocation");
		return (-1);
	}
	return (0);
}

void	child_process_pipes(t_cmd *cur, t_shell_env *env, t_pipe_info *pipes)
{
	handle_redirections(cur);
	if (pipes->prev_fd != -1)
	{
		dup2(pipes->prev_fd, STDIN_FILENO);
		close(pipes->prev_fd);
	}
	if (cur->next)
	{
		dup2(pipes->fd[1], STDOUT_FILENO);
		close(pipes->fd[1]);
		close(pipes->fd[0]);
	}
	if (is_builtin(env, cur->av[0]))
		exit(exec_builtin(cur, env));
	exit(exec_external(cur, env));
}

void	parent_process_pipes(t_pipe_info *pipes, t_cmd *cur)
{
	if (pipes->prev_fd != -1)
		close(pipes->prev_fd);
	if (cur->next)
	{
		close(pipes->fd[1]);
		pipes->prev_fd = pipes->fd[0];
	}
}

void	wait_for_children(pid_t *pids, int count, int *status, t_shell_env *env)
{
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], status, 0);
		i++;
	}
	if (WIFEXITED(*status))
		env->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		env->exit_status = WTERMSIG(*status);
}
