/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:39:21 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 17:05:25 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_cmd **cur_cmd)
{
	if (!(*cur_cmd)->av || (*cur_cmd)->av[0] == NULL)
	{
		ft_putendl_fd("Minishell: syntax error near unexpected token `|'", 2);
		return (0);
	}
	(*cur_cmd)->next = malloc(sizeof(t_cmd));
	if (!(*cur_cmd)->next)
		return (0);
	*cur_cmd = (*cur_cmd)->next;
	(*cur_cmd)->av = NULL;
	(*cur_cmd)->redirs = NULL;
	(*cur_cmd)->next = NULL;
	return (1);
}

int	handle_redir(char *token, t_data *data, t_shell_env *env, t_cmd **cur_cmd)
{
	t_redir_type	type;
	char			*filename;

	type = get_redir_type(token);
	filename = get_next_token(&data->line, env, data);
	if (!filename)
	{
		ft_putendl_fd("Minishell: syntax error: unexpected newline", 2);
		free(token);
		return (0);
	}
	add_redir_to_cmd(*cur_cmd, type, filename);
	free(filename);
	free(token);
	return (1);
}

int	process_token(char *token, t_data *data, t_shell_env *env, t_cmd **cur_cmd)
{
	if (!ft_strcmp(token, "|"))
	{
		free(token);
		if (!handle_pipe(cur_cmd))
			return (0);
	}
	else if (is_redir(token))
	{
		if (!handle_redir(token, data, env, cur_cmd))
			return (0);
	}
	else
	{
		(*cur_cmd)->av = add_to_argv((*cur_cmd)->av, token);
		free(token);
	}
	return (1);
}
