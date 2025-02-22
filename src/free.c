/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:53:09 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 12:58:17 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Libère une liste chaînée de redirections
	On libère d'abord le nom du fichier qui a été alloué
	Puis la structure de redirection elle-même
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

/*
	Libère le tableau d'arguments d'une commande
	On parcourt le tableau jusqu'à NULL
	Libération de chaque argument
	Libération du tableau lui-même
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

/*
	Fonction principale qui libère toute la liste de commandes
	On libère d'abord les redirections de la commande
	Puis les arguments
	Enfin la structure de commande elle-même
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
