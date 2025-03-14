/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:34:06 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/14 21:25:37 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie la validité d'un nom pour unset.
 *
 * Le nom ne doit pas être vide, commencer par '=' ou un chiffre,
 * et doit être composé d'alphanumériques ou '_'.
 *
 * @param args Nom à vérifier.
 * @return int 1 si valide, 0 sinon.
 */
static int	check_unset_name(char *args)
{
	int	i;

	if (!args || !args[0] || args[0] == '=' || ft_isdigit(args[0]))
		return (0);
	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Vérifie un argument de unset.
 *
 * Si l'argument n'est pas un identifiant valide, affiche une erreur.
 *
 * @param arg Argument à vérifier.
 * @return int 0 si valide, 1 sinon.
 */
static int	check_unset_arg(char *arg)
{
	if (!check_unset_name(arg))
	{
		ft_printf("minishell: unset: `%s': not a valid identifier\n", arg);
		return (1);
	}
	return (0);
}

/**
 * @brief Renvoie l'index d'une variable dans env.
 *
 * Compare var avec chaque élément de env en 
 * s'assurant que le nom est suivi de '='.
 *
 * @param env Tableau d'environnement.
 * @param var Nom de la variable.
 * @return int Index de la variable, ou -1 si introuvable.
 */
static int	env_var_index(char **env, char *var)
{
	int	i;
	int	len;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Supprime une variable de l'environnement.
 *
 * Libère la variable à l'index donné et décale le reste du tableau.
 *
 * @param env Tableau d'environnement.
 * @param index Index de la variable à supprimer.
 */
static void	remove_env_var(char **env, int index)
{
	free(env[index]);
	while (env[index])
	{
		env[index] = env[index + 1];
		index++;
	}
}

/**
 * @brief Implémente la commande unset.
 *
 * Parcourt les arguments et supprime la variable
 * correspondante dans l'environnement.
 *
 * @param args Tableau d'arguments.
 * @param shell_env Environnement du shell.
 * @return int 0 si succès, 1 en cas d'erreur.
 */
int	ft_unset(char **args, t_shell_env *shell_env)
{
	int	i;
	int	index;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (check_unset_arg(args[i]))
			return (1);
		index = env_var_index(shell_env->env, args[i]);
		if (index != -1)
			remove_env_var(shell_env->env, index);
		i++;
	}
	return (0);
}
