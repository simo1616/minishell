/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:54:57 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 21:06:49 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Construit un chemin complet pour une commande.
 *
 * Si path se termine par '/', concatène directement cmd, sinon
 * insère un '/' entre path et cmd.
 *
 * @param path Chemin de base.
 * @param cmd Nom de la commande.
 * @return char* Chemin complet alloué.
 */
static char	*build_path_sp(char *path, char *cmd)
{
	int	len;

	len = ft_strlen(path);
	if (path[len - 1] == '/')
		return (ft_strjoin(path, cmd));
	else
		return (ft_strjoin_three(path, "/", cmd));
}

/**
 * @brief Cherche une commande dans le PATH.
 *
 * Découpe path_env par ':' et teste l'exécutabilité de chaque chemin.
 *
 * @param cmd Nom de la commande.
 * @param path_env Variable d'environnement PATH.
 * @return char* Chemin trouvé ou NULL.
 */
char	*search_in_path(char *cmd, char *path_env)
{
	char		**path_sp;
	char		*str;
	int			i;

	path_sp = ft_split(path_env, ':');
	if (!path_sp)
		return (NULL);
	i = 0;
	while (path_sp[i])
	{
		str = build_path_sp(path_sp[i], cmd);
		if (access(str, X_OK) == 0)
		{
			free_av(path_sp);
			return (str);
		}
		free(str);
		i++;
	}
	free_av(path_sp);
	return (NULL);
}

/**
 * @brief Résout le chemin d'une commande.
 *
 * Si cmd contient '/', vérifie l'exécutabilité directement.
 * Sinon, cherche la commande dans PATH.
 *
 * @param cmd Nom de la commande.
 * @param env Tableau des variables d'environnement.
 * @return char* Chemin résolu ou NULL.
 */
char	*resolve_path(char *cmd, char **env)
{
	char	*path_env;
	int		i;

	if (!env)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = NULL;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			path_env = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	return (search_in_path(cmd, path_env));
}
