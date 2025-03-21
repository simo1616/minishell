/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:01:38 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 21:26:58 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Renvoie l'index d'une variable dans l'environnement.
 * @param shell_env Environnement du shell.
 * @param name Nom de la variable.
 * @return int Index de la variable ou -1 si non trouvée.
 */
int	find_env_index(t_shell_env *shell_env, const char *name)
{
	int	i;
	int	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (shell_env->env[i])
	{
		if (ft_strncmp(shell_env->env[i], name, name_len) == 0
			&& shell_env->env[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Récupère la valeur d'une variable d'environnement.
 * @param shell_env Environnement du shell.
 * @param name Nom de la variable.
 * @return char* Valeur de la variable ou NULL si inexistante.
 */
char	*env_get(t_shell_env *shell_env, const char *name)
{
	int		index;
	char	*sign_egal;

	index = find_env_index(shell_env, name);
	if (index == -1)
		return (NULL);
	sign_egal = ft_strchr(shell_env->env[index], '=');
	if (!sign_egal)
		return (NULL);
	return (sign_egal + 1);
}

/**
 * @brief Concatène trois chaînes : nom, "=", et valeur.
 * @param name Nom de la variable.
 * @param eq Chaîne contenant "=".
 * @param value Valeur de la variable.
 * @return char* Chaîne concaténée ou NULL en cas d'erreur.
 */
char	*ft_strjoin_three(const char *name, const char *eq, const char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(name, eq);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
}

/**
 * @brief Supprime une variable de l'environnement.
 * @param shell_env Environnement du shell.
 * @param name Nom de la variable à supprimer.
 * @return int 0 si succès, 1 si la variable n'est pas trouvée.
 */
int	env_unset(t_shell_env *shell_env, const char *name)
{
	int	index;

	index = find_env_index(shell_env, name);
	if (index == -1)
		return (1);
	free(shell_env->env[index]);
	while (shell_env->env[index + 1])
	{
		shell_env->env[index] = shell_env->env[index + 1];
		index++;
	}
	shell_env->env[index] = NULL;
	return (0);
}
