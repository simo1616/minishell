/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/27 15:15:19 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_cmd *cmds, t_shell_env *env)
{
	t_cmd	*current;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;
	int		saved_stdin;
	int		saved_stdout;
	t_redir	*redir;

	redir = cmds->redirs;
	prev_fd = -1;
	current = cmds;
	while (current)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_redirections(current);
		if (handle_redirections(current) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redir->filename, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			env->exit_status = 1;
			return ;
		}
		if (!current->next && is_builtin(env, current->av[0]))
			env->exit_status = exec_builtin(current, env);
		else
		{
			if (current->next && pipe(pipe_fd) == -1)
				return (perror("pipe"));
			pid = fork();
			if (pid == -1)
				return (perror("fork"));
			if (pid == 0)
			{
				if (prev_fd != -1)
				{
					dup2(prev_fd, STDIN_FILENO);
					close(prev_fd);
				}
				if (current->next)
				{
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
					close(pipe_fd[0]);
				}
				if (is_builtin(env, current->av[0]))
					exit(exec_builtin(current, env));
				else
					exit(exec_external(current, env));
			}
			else
			{
				if (prev_fd != -1)
					close(prev_fd);
				if (current->next)
				{
					close(pipe_fd[1]);
					prev_fd = pipe_fd[0];
				}
				waitpid(pid, &env->exit_status, 0);
			}
		}
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
}
