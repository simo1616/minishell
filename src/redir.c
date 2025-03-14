/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/14 20:19:00 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si token est une redirection.
 *
 * Compare token à "<", ">", "<<" ou ">>".
 *
 * @param token Chaîne à tester.
 * @return int 1 si c'est une redirection, 0 sinon.
 */
int	is_redir(char *token)
{
	return (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0);
}

/**
 * @brief Retourne le type de redirection.
 *
 * Associe token à REDIR_IN, REDIR_OUT, REDIR_APPEND ou 
 * REDIR_HEREDOC.
 *
 * @param token Chaîne représentant la redirection.
 * @return t_redir_type Type de redirection.
 */
t_redir_type	get_redir_type(char *token)
{
	if (ft_strcmp(token, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(token, ">") == 0)
		return (REDIR_OUT);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIR_APPEND);
	if (ft_strcmp(token, "<<") == 0)
		return (REDIR_HEREDOC);
	return (REDIR_OUT);
}

/**
 * @brief Ajoute une redirection à une commande.
 *
 * Alloue un nouveau t_redir, copie le type et le nom du fichier,
 * puis l'ajoute à la fin de la liste.
 *
 * @param cmd Commande à modifier.
 * @param type Type de redirection.
 * @param filename Nom du fichier.
 */
void	add_redir_to_cmd(t_cmd *cmd, t_redir_type type, char *filename)
{
	t_redir	*new_rd;
	t_redir	*tmp;

	new_rd = malloc(sizeof(t_redir));
	if (!new_rd)
		return ;
	new_rd->type = type;
	new_rd->filename = ft_strdup(filename);
	new_rd->next = NULL;
	if (cmd->redirs == NULL)
		cmd->redirs = new_rd;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_rd;
	}
}
