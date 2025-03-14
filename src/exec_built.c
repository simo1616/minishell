/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:48:06 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 20:33:44 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si une commande est builtin.
 *
 * Parcourt le tableau des builtins et compare le nom de chaque
 * builtin avec cmd_name.
 *
 * @param env Environnement du shell.
 * @param cmd_name Nom de la commande.
 * @return int 1 si c'est un builtin, 0 sinon.
 */
int	is_builtin(t_shell_env *env, char *cmd_name)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtins(env);
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, cmd_name) == 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Exécute une commande builtin.
 *
 * Parcourt le tableau des builtins et si la commande correspond,
 * exécute la fonction associée avec les arguments et l'environnement.
 *
 * @param cmd Structure de commande.
 * @param env Environnement du shell.
 * @return int Résultat de l'exécution du builtin, ou 0 si non trouvé.
 */
int	exec_builtin(t_cmd *cmd, t_shell_env *env)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtins(env);
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, cmd->av[0]) == 0)
			return (builtins[i].func(cmd->av, env));
		i++;
	}
	return (0);
}
