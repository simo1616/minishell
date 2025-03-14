/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:24:10 by jdecarro          #+#    #+#             */
/*   Updated: 2025/01/31 14:24:10 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche le répertoire de travail courant.
 *
 * Utilise getcwd pour récupérer le chemin et l'affiche.
 * En cas d'échec, affiche l'erreur avec perror.
 *
 * @param argv Non utilisé.
 * @param shell_env Non utilisé.
 * @return int 0 en cas de succès, 1 en cas d'erreur.
 */
int	ft_pwd(char **argv, t_shell_env *shell_env)
{
	char	cwd[PATH_MAX];

	(void)argv;
	(void)shell_env;
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
