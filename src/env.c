/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:59:17 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 15:47:17 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplique le tableau d'environnement.
 *
 * Alloue et retourne un nouveau tableau de chaînes de caractères contenant
 * une copie de chaque variable d'environnement passée en paramètre.
 *
 * @param envp Tableau d'environnement original.
 * @return char** Nouveau tableau d'environnement ou NULL en cas d'erreur.
 */
static char	**dup_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (--i >= 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

/**
 * @brief Crée l'environnement du shell.
 *
 * Alloue et initialise une structure t_shell_env en dupliquant les variables
 * d'environnement fournies et en initialisant les autres champs.
 *
 * @param envp Tableau d'environnement original.
 * @return t_shell_env* Pointeur vers l'environnement du shell
 * ou NULL en cas d'erreur.
 */
t_shell_env	*create_shell_env(char **envp)
{
	t_shell_env	*shell_env;

	shell_env = malloc(sizeof(t_shell_env));
	if (!shell_env)
		return (NULL);
	shell_env->env = dup_env(envp);
	if (!shell_env->env)
	{
		free(shell_env);
		return (NULL);
	}
	shell_env->exit_status = 0;
	shell_env->running = 1;
	shell_env->builtins = NULL;
	return (shell_env);
}

/**
 * @brief Détruit l'environnement du shell.
 *
 * Libère toutes les ressources associées à la structure t_shell_env,
 * y compris le tableau d'environnement et les builtins.
 *
 * @param shell_env Pointeur vers l'environnement du shell à détruire.
 */
void	destroy_shell_env(t_shell_env *shell_env)
{
	int	i;

	if (!shell_env)
		return ;
	if (shell_env->env)
	{
		i = 0;
		while (shell_env->env[i])
			free(shell_env->env[i++]);
		free(shell_env->env);
	}
	if (shell_env->builtins)
	{
		free_builtins(shell_env->builtins);
	}
	free(shell_env);
}
