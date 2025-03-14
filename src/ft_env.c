/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:43:00 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/14 21:20:30 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche les variables d'environnement.
 *
 * Parcourt le tableau shell_env->env et imprime chaque variable.
 *
 * @param argv Non utilisé.
 * @param shell_env Environnement du shell.
 * @return int Retourne 0.
 */
int	ft_env(char **argv, t_shell_env *shell_env)
{
	int	i;

	(void)argv;
	i = 0;
	while (shell_env->env[i])
	{
		printf("%s\n", shell_env->env[i]);
		i++;
	}
	return (0);
}
