/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:38:15 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 21:01:01 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise les entrées des builtins.
 *
 * Remplit le tableau builtins avec les noms des builtins
 * et pointeurs de fonctions pour chacunes.
 *
 * @param builtins Tableau des builtins à initialiser.
 */
static void	init_builtin_entries(t_builtin *builtins)
{
	builtins[0].name = ft_strdup("cd");
	builtins[0].func = &ft_cd;
	builtins[1].name = ft_strdup("exit");
	builtins[1].func = &ft_exit;
	builtins[2].name = ft_strdup("pwd");
	builtins[2].func = &ft_pwd;
	builtins[3].name = ft_strdup("echo");
	builtins[3].func = &ft_echo;
	builtins[4].name = ft_strdup("env");
	builtins[4].func = &ft_env;
	builtins[5].name = ft_strdup("export");
	builtins[5].func = &ft_export;
	builtins[6].name = ft_strdup("unset");
	builtins[6].func = &ft_unset;
	builtins[7].name = NULL;
	builtins[7].func = NULL;
}

/**
 * @brief Initialise et retourne le tableau des builtins.
 *
 * Si shell_env->builtins n'est pas encore alloué, alloue un tableau
 * de 8 entrées et appelle init_builtin_entries. En cas d'erreur
 * d'allocation, affiche une erreur et quitte.
 *
 * @param shell_env Environnement du shell.
 * @return t_builtin* Tableau des builtins.
 */
t_builtin	*init_builtins(t_shell_env *shell_env)
{
	if (shell_env->builtins == NULL)
	{
		shell_env->builtins = malloc(sizeof(t_builtin) * 8);
		if (!shell_env->builtins)
		{
			perror("error malloc builtins");
			exit(EXIT_FAILURE);
		}
		init_builtin_entries(shell_env->builtins);
	}
	return (shell_env->builtins);
}
