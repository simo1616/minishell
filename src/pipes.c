/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:25:52 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 19:28:54 by mbendidi         ###   ########.fr       */
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

// static void debug_print_cmd(t_cmd *cmd)
// {
//     int i = 0;
//     printf("Commande:\n");
//     while (cmd->av[i])
// 	{
//         printf("  av[%d] = [%s]\n", i, cmd->av[i]);
//         i++;
//     }
//     t_redir *redir = cmd->redirs;
//     while (redir)
// 	{
//         printf("  redir: type = %d, filename = [%s].\n", redir->type,
	//redir->filename);
//         redir = redir->next;
//     }
// }

int	excec_pipes(t_cmd *cmds, t_shell_env *env)
{
	t_cmd	*cur;
	pid_t	*pids;
	int		prev_fd;
	int		fd[2];
	int		status;
	int		i;
	int		j;
	int		ret;
	t_redir	*redir;

	cur = cmds;
	prev_fd = -1;
	pids = malloc(sizeof(pid_t) * ft_lstsize_cmd(cur));
	if (!pids)
	{
		perror("Erreur d'allocation des PIDs");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (cur)
	{
		if (cur->next)
		{
			if (pipe(fd) < 0)
			{
				perror("Erreur lors de la création du pipe");
				free(pids);
				exit(EXIT_FAILURE);
			}
		}
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("Erreur lors du fork");
			free(pids);
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cur->next)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			if (cur->av && cur->av[0])
			{
				if (is_builtin(env, cur->av[0]))
				{
					ret = exec_builtin(cur, env);
					env->exit_status = ret;
				}
				else
				{
					ret = excec_external(cur, env);
					env->exit_status = ret;
				}
			}
			redir = cur->redirs;
			while (redir)
			{
				printf("\nRedirection: type=%d, file=%s\n", redir->type,
					redir->filename);
				redir = redir->next;
			}
			exit(0); // exit_status ?
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cur->next)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		cur = cur->next;
		i++;
	}
	j = 0;
	while (j < i)
	{
		waitpid(pids[j], &status, 0);
		j++;
	}
	close(fd[0]);
	if (WIFEXITED(status))
		env->exit_status = WTERMSIG(status);
	else if (WIFSIGNALED(status))
		env->exit_status = WTERMSIG(status);
	free(pids);
	return (env->exit_status);
}

// cmd1 | cmd2 | cmd3 | cmd4 | cmd5