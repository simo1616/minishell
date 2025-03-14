/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:36:07 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 21:06:12 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Exécute le processus enfant pour une commande externe.
 *
 * Remplace le comportement de SIGQUIT, puis lance execve.
 * En cas d'erreur, affiche un message, libère path et quitte.
 *
 * @param path Chemin de la commande.
 * @param cmd Commande à exécuter.
 * @param shell_env Environnement du shell.
 */
static void	exec_child_process(char *path, t_cmd *cmd, t_shell_env *shell_env)
{
	signal(SIGQUIT, SIG_DFL);
	execve(path, cmd->av, shell_env->env);
	perror("minishell: error Child process\n");
	free(path);
	exit(EXIT_FAILURE);
}

/**
 * @brief Gère le cas où la commande n'est pas trouvée.
 *
 * Affiche un message d'erreur indiquant que la commande n'a pas été trouvée.
 *
 * @param cmd_name Nom de la commande.
 * @return int Code d'erreur EX_CMD_NT_FD.
 */
static int	handle_no_path(char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (EX_CMD_NT_FD);
}

/**
 * @brief Gère une erreur lors du fork.
 *
 * Affiche l'erreur, met à jour le statut de sortie,
 * libère path et retourne ce statut.
 *
 * @param path Chemin de la commande.
 * @param shell_env Environnement du shell.
 * @return int Code de sortie mis à jour.
 */
static int	handle_fork_error(char *path, t_shell_env *shell_env)
{
	perror("minishell");
	shell_env->exit_status = 1;
	free(path);
	return (shell_env->exit_status);
}

/**
 * @brief Met à jour le statut de sortie de l'environnement.
 *
 * Si le processus s'est terminé normalement, récupère le code de sortie.
 * Sinon, ajoute un offset au numéro du signal.
 *
 * @param shell_env Environnement du shell.
 * @param status Statut renvoyé par waitpid.
 */
void	update_exit_status(t_shell_env *shell_env, int status)
{
	if (WIFEXITED(status))
		shell_env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell_env->exit_status = EXIT_SIG_OFFSET + WTERMSIG(status);
}

/**
 * @brief Exécute une commande externe.
 *
 * Résout le chemin de la commande, vérifie qu'elle n'est pas un répertoire,
 * fork le processus et attend sa fin. Met à jour le statut de sortie.
 *
 * @param cmd Commande à exécuter.
 * @param shell_env Environnement du shell.
 * @return int Code de sortie.
 */
int	exec_external(t_cmd *cmd, t_shell_env *shell_env)
{
	pid_t		pid;
	char		*path;
	int			status;
	struct stat	sb;

	path = resolve_path(cmd->av[0], shell_env->env);
	if (!path)
		return (handle_no_path(cmd->av[0]));
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		free(path);
		return (EX_CMD_NT_EXE);
	}
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(path, shell_env));
	if (pid == 0)
		exec_child_process(path, cmd, shell_env);
	waitpid(pid, &status, 0);
	free(path);
	update_exit_status(shell_env, status);
	return (shell_env->exit_status);
}
