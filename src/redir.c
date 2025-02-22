/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/22 17:11:40 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(char *token)
{
	return (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
	ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0);
}

t_redir_type	get_redir_type(char *token) //changer t_redir_type par int
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

void	add_redir_to_cmd(t_cmd *cmd, t_redir_type type, char *filename)
{
	t_redir *new_rd;
	t_redir *tmp;

	new_rd = malloc(sizeof(t_redir));
	if (!new_rd)
		return;
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

