/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:25:52 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/27 12:50:15 by jdecarro         ###   ########.fr       */
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

int	exec_pipes(t_cmd *cmds, t_shell_env *env)
{
	t_cmd	*cur;
	pid_t	*pids;
	int		prev_fd;
	int		fd[2];
	int		status;
	int		i;
	int		cmd_count;

	prev_fd = -1;
	cmd_count = ft_lstsize_cmd(cmds);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		perror("Error PID allocation");
		exit(EXIT_FAILURE);
	}
	i = 0;
	cur = cmds;
	while (cur)
	{
		if (cur->next && pipe(fd) < 0)
		{
			perror("Error pipe");
			free(pids);
			exit(EXIT_FAILURE);
		}
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("Error fork");
			free(pids);
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			handle_redirections(cur);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO); //Lire depuis le pipe précédent
				close(prev_fd);
			}
			if (cur->next)
			{
				dup2(fd[1], STDOUT_FILENO); //Écrire dans le pipe suivant
				close(fd[1]);
				close(fd[0]);
			}
			if (is_builtin(env, cur->av[0]))
				exit(exec_builtin(cur, env));
			else
				exit(exec_external(cur, env));
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd); //Fermer le fd prev
			if (cur->next)
			{
				close(fd[1]); // Fermer le writedu pipe
				prev_fd = fd[0]; // Garder le read pour la prochaine commande
			}
		}
		cur = cur->next;
		i++;
	}
	while (i-- > 0)
		waitpid(pids[i], &status, 0);
	if (WIFEXITED(status))
		env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		env->exit_status = WTERMSIG(status);
	free(pids);
	return (env->exit_status);
}
