/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:50:28 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/25 21:30:23 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_cmd_ifneed(t_cmd **cmd, t_cmd **cur_cmd)
{
	if (!*cmd)
	{
		*cmd = malloc(sizeof(t_cmd));
		if (!*cmd)
			return (0);
		*cur_cmd = *cmd;
		(*cur_cmd)->next = NULL;
		(*cur_cmd)->av = NULL;
		(*cur_cmd)->redirs = NULL;
	}
	return (1);
}

t_cmd	*parse_tokens(t_data *data, t_shell_env *env)
{
	t_cmd	*cmd;
	t_cmd	*cur_cmd;
	char	*token;

	cmd = NULL;
	cur_cmd = NULL;
	while (1)
	{
		token = get_next_token(env, data);
		if (!token)
			break ;
		if (!init_cmd_ifneed(&cmd, &cur_cmd))
		{
			free_cmds(cmd);
			return (NULL);
		}
		if (!process_token(token, data, env, &cur_cmd))
		{
			free_cmds(cmd);
			return (NULL);
		}
	}
	return (cmd);
}

int	final_verification(t_cmd *cmd)
{
	t_cmd	*cur_cmd;

	cur_cmd = cmd;
	while (cur_cmd->next)
		cur_cmd = cur_cmd->next;
	if (!cur_cmd->av || cur_cmd->av[0] == NULL)
	{
		ft_putendl_fd("Minishell: syntax error near unexpected token `|'", 2);
		free_cmds(cmd);
		return (0);
	}
	return (1);
}

t_cmd	*parse_command_line(char *line, t_shell_env *env)
{
	t_data	data;
	t_cmd	*cmd;

	if (!line)
		return (NULL);
	data.line = line;
	data.cpos = 0;
	if (!get_quotes_context(&data))
		return (NULL);
	cmd = parse_tokens(&data, env);
	if (!cmd)
	{
		free(data.ctx);
		return (NULL);
	}
	if (!final_verification(cmd))
	{
		free(data.ctx);
		return (NULL);
	}
	free(data.ctx);
	return (cmd);
}
