/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:52:03 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/11 14:52:03 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Exécute la commande dans le processus enfant.
 *
 * Configure les redirections en dupliquant les FDs, puis
 * exécute un builtin ou une commande externe et quitte.
 *
 * @param cmd Commande à exécuter.
 * @param env Environnement du shell.
 * @param pipes Structure contenant les informations de pipe.
 */
void	child_process(t_cmd *cmd, t_shell_env *env, t_pipe_info *pipes)
{
	if (pipes->prev_fd != -1)
	{
		dup2(pipes->prev_fd, STDIN_FILENO);
		close(pipes->prev_fd);
	}
	if (cmd->next)
	{
		dup2(pipes->fd[1], STDOUT_FILENO);
		close(pipes->fd[1]);
		close(pipes->fd[0]);
	}
	if (is_builtin(env, cmd->av[0]))
		exit(exec_builtin(cmd, env));
	exit(exec_external(cmd, env));
}

/**
 * @brief Gère le processus parent après un fork.
 *
 * Ferme les FDs inutiles, attend la fin du fils, met à jour
 * le statut de sortie et retourne le dernier FD de pipe.
 *
 * @param pid PID du processus enfant.
 * @param pipes Structure des pipes.
 * @param cmd Commande en cours d'exécution.
 * @param env Environnement du shell.
 * @return int Dernier descripteur de pipe (prev_fd).
 */
int	parent_process(pid_t pid, t_pipe_info *pipes, t_cmd *cmd, t_shell_env *env)
{
	int	status;

	if (pipes->prev_fd != -1)
		close(pipes->prev_fd);
	if (cmd->next)
	{
		close(pipes->fd[1]);
		pipes->prev_fd = pipes->fd[0];
	}
	waitpid(pid, &status, 0);
	update_exit_status(env, status);
	return (pipes->prev_fd);
}

/**
 * @brief Restaure les descripteurs standards.
 *
 * Remplace STDIN et STDOUT par leurs copies sauvegardées,
 * puis ferme les copies.
 *
 * @param saved_stdin Sauvegarde de STDIN.
 * @param saved_stdout Sauvegarde de STDOUT.
 */
void	reset_std(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Ferme le descripteur de pipe précédent s'il est ouvert.
 *
 * @param prev_fd Descripteur à fermer.
 */
void	close_prev_fd(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
}

/**
 * @brief Fork et exécute une commande.
 *
 * Si la commande est suivie d'une autre, crée un pipe. Effectue un fork :
 * dans le fils, exécute child_process, sinon, met à jour le FD via 
 * parent_process.
 *
 * @param cmd Commande à exécuter.
 * @param env Environnement du shell.
 * @param pipes Structure des pipes.
 * @return int Dernier descripteur de pipe ou -1 en cas d'erreur.
 */
int	fork_and_exec(t_cmd *cmd, t_shell_env *env, t_pipe_info *pipes)
{
	pid_t	pid;

	if (cmd->next && pipe(pipes->fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		child_process(cmd, env, pipes);
	else
		pipes->prev_fd = parent_process(pid, pipes, cmd, env);
	return (pipes->prev_fd);
}
