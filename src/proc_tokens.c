/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:39:21 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/28 22:23:33 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_cmd **cur_cmd)
{
	if (!(*cur_cmd)->av || (*cur_cmd)->av[0] == NULL)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
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

int handle_redir(char *token, t_data *data, t_shell_env *env, t_cmd **cur_cmd)
{
	t_redir_type	type;
	t_token_data	filename_data;
	char			*filename;

	type = get_redir_type(token);
	filename_data = get_next_token(env, data);
	if (!filename_data.token || is_redir(filename_data.token) || 
		!ft_strcmp(filename_data.token, "|"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (filename_data.token)
			ft_putstr_fd(filename_data.token, 2);
		else
			ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
		if (filename_data.token)
			free(filename_data.token);
		return (0);
	}
	filename = filename_data.token;
	add_redir_to_cmd(*cur_cmd, type, filename);
	free(filename_data.token_ctx);
	return (1);
}

int process_token(char *token, t_data *data, t_shell_env *env, t_cmd **cur_cmd, int *token_ctx, size_t len)
{
	size_t	i;
	int		quote;
	int		result;

	i = 0;
	quote = 1;
	while (i < len)
	{
		if (token_ctx[i] == 0)
		{
			quote = 0;
			break;
		}
		i++;
	}
	if (!quote && (!ft_strcmp(token, "|") || is_redir(token)))
	{
		if (!ft_strcmp(token, "|"))
        {
            result = handle_pipe(cur_cmd);
            free(token);
            return (result);
        }
		else
		{
			while (token && is_redir(token))
			{
				result = handle_redir(token, data, env, cur_cmd);
				free(token);
				if (!result)
					return (0);
				token = get_next_token(env, data).token;
			}
			if (token && !ft_strcmp(token, "|"))
			{
				result = handle_pipe(cur_cmd);
				free(token);
				return (result);
			}
			if (token)
			{
				(*cur_cmd)->av = add_to_argv((*cur_cmd)->av, token);
				free(token);
			}
			return (1);
		}
	}
	else
	{
		(*cur_cmd)->av = add_to_argv((*cur_cmd)->av, token);
		free(token);
	}
	return (1);
}