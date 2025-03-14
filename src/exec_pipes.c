/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 09:25:52 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 20:58:20 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise un pipe si nécessaire.
 *
 * Si la commande courante a un successeur, crée un pipe.
 * En cas d'erreur, affiche un message et retourne -1.
 *
 * @param pipes Structure de pipe.
 * @param cur Commande courante.
 * @return int 0 si succès, -1 en cas d'erreur.
 */
static int	setup_pipe(t_pipe_info *pipes, t_cmd *cur)
{
	if (cur->next && pipe(pipes->fd) < 0)
	{
		perror("error pipe");
		return (-1);
	}
	return (0);
}

/**
 * @brief Exécute des commandes chaînées avec des pipes.
 *
 * Initialise les pipes, effectue un fork pour chaque commande,
 * puis attend la fin des processus enfants. Retourne le code
 * de sortie final.
 *
 * @param cmds Liste chaînée des commandes.
 * @param env Environnement du shell.
 * @return int Code de sortie (exit_status).
 */
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
