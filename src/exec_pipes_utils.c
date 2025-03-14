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

/**
 * @brief Calcule la taille d'une liste de commandes.
 * @param cmds Liste chaînée de commandes.
 * @return int Nombre d'éléments.
 */
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

/**
 * @brief Initialise la structure de pipe.
 *
 * Définit prev_fd à -1, compte les commandes, et alloue
 * un tableau de PID.
 *
 * @param pipes Structure de pipe.
 * @param cmds Liste de commandes.
 * @return int 0 si succès, -1 en cas d'erreur.
 */
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

/**
 * @brief Processus enfant pour l'exécution en pipe.
 *
 * Gère les redirections, duplique les FDs, et exécute la commande.
 *
 * @param cur Commande courante.
 * @param env Environnement du shell.
 * @param pipes Structure de pipe.
 */
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

/**
 * @brief Processus parent pour l'exécution en pipe.
 *
 * Ferme le FD précédent et, si la commande a un successeur,
 * met à jour prev_fd.
 *
 * @param pipes Structure de pipe.
 * @param cur Commande courante.
 */
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

/**
 * @brief Attend la fin des processus enfants.
 *
 * Appelle waitpid pour chaque PID et met à jour le statut.
 *
 * @param pids Tableau des PID.
 * @param count Nombre de processus.
 * @param status Pointeur sur le status.
 * @param env Environnement du shell.
 */
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
