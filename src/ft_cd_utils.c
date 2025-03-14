/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:39:15 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/11 15:39:15 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Retourne le chemin à utiliser pour cd.
 *
 * Si aucun argument ou "~" est fourni, retourne HOME.
 *
 * @param args Tableau d'arguments.
 * @param shell_env Environnement du shell.
 * @return char* Chemin à utiliser.
 */
char	*get_cd_path(char **args, t_shell_env *shell_env)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0 || ft_strcmp(args[1], "") == 0)
		return (env_get(shell_env, "HOME"));
	return (args[1]);
}

/**
 * @brief Vérifie le nombre d'arguments pour cd.
 *
 * Retourne -1 et affiche une erreur si plus d'un argument est donné.
 *
 * @param args Tableau d'arguments.
 * @param shell_env Environnement du shell.
 * @return int 0 si OK, -1 sinon.
 */
int	check_cd_args(char **args, t_shell_env *shell_env)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell_env->exit_status = 1;
		return (-1);
	}
	return (0);
}

/**
 * @brief Valide que le chemin est un répertoire.
 *
 * Utilise stat() pour vérifier le type du chemin.
 *
 * @param path Chemin à valider.
 * @param shell_env Environnement du shell.
 * @return int 0 si OK, -1 sinon.
 */
int	validate_cd_path(char *path, t_shell_env *shell_env)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode))
		return (print_cd_error(path, "Not a directory", shell_env));
	return (0);
}

/**
 * @brief Affiche une erreur lors du cd.
 *
 * Affiche le message d'erreur et met à jour le statut de sortie.
 *
 * @param path Chemin invalide.
 * @param error_msg Message d'erreur.
 * @param shell_env Environnement du shell.
 * @return int Retourne 1.
 */
int	print_cd_error(char *path, char *error_msg, t_shell_env *shell_env)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\n", 2);
	shell_env->exit_status = 1;
	return (1);
}

/**
 * @brief Met à jour les variables d'environnement PWD et OLDPWD.
 *
 * Affecte OLDPWD à l'ancienne valeur de PWD et met PWD à new_pwd.
 *
 * @param shell_env Environnement du shell.
 * @param new_pwd Nouveau répertoire courant.
 */
void	update_env_pwd(t_shell_env *shell_env, char *new_pwd)
{
	env_set(shell_env, "OLDPWD", env_get(shell_env, "PWD"));
	env_set(shell_env, "PWD", new_pwd);
}
