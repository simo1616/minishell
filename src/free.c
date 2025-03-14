/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:53:09 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 16:57:34 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Libère le tableau des builtins.
 *
 * Parcourt le tableau des builtins et libère le nom de chaque builtin, 
 * puis libère le tableau lui-même.
 *
 * @param builtins Tableau des builtins.
 */
void	free_builtins(t_builtin *builtins)
{
	int	i;

	i = 0;
	if (!builtins)
		return ;
	while (builtins[i].name)
	{
		free(builtins[i].name);
		i++;
	}
	free(builtins);
}

/**
 * @brief Libère une liste chaînée de redirections.
 *
 * Libère d'abord le nom de fichier associé à chaque redirection, puis 
 * libère chaque maillon de la liste.
 *
 * @param redirs Liste chaînée de redirections.
 */
void	free_redirs(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		free(current);
		current = next;
	}
}

/**
 * @brief Libère un tableau d'arguments.
 *
 * Parcourt le tableau d'arguments jusqu'à NULL, libère chaque argument, 
 * puis libère le tableau lui-même.
 *
 * @param argv Tableau d'arguments.
 */
void	free_av(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/**
 * @brief Libère une liste chaînée de commandes.
 *
 * Libère d'abord les redirections de chaque commande, ensuite le 
 * tableau d'arguments, et enfin la commande elle-même.
 *
 * @param cmds Liste chaînée de commandes.
 */
void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		next = current->next;
		if (current->redirs)
			free_redirs(current->redirs);
		if (current->av)
			free_av(current->av);
		free(current);
		current = next;
	}
}
