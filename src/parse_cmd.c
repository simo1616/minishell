#include "minishell.h"

t_cmd	*parse_command_line(char *line, t_shell_env *env)
{
	t_cmd			*cmd;
	t_cmd			*cur_cmd;
	t_data			data;
	t_redir_type	type;
	char			*token;
	char			*filename;

	if (!line)
		return (NULL);
	cmd = NULL;
	cur_cmd = NULL;
	data.line = line;
	data.cpos = 0;
	if (!get_quotes_context(&data))
	{
		free(cmd);
		return (NULL);
	}
	while (1)
	{
		token = get_next_token(&data.line, env, &data);
		if (!token)
			break ;
		if (!cmd)
		{
			cmd = malloc(sizeof(t_cmd));
			if (!cmd)
				return (NULL);
			cmd->av = NULL;
			cmd->redirs = NULL;
			cmd->next = NULL;
			cur_cmd = cmd;
		}
		if (!ft_strcmp(token, "|"))
		{
			free(token);
			if (!cur_cmd->av || cur_cmd->av[0] == NULL)
			{
				ft_putendl_fd("Minishell: syntax error near unexpected token `|'",
					2);
				free_cmds(cmd);
				free(data.ctx);
				return (NULL);
			}
			cur_cmd->next = malloc(sizeof(t_cmd));
			if (!cur_cmd)
			{
				free_cmds(cmd);
				free(data.ctx);
				return (NULL);
			}
			cur_cmd = cur_cmd->next;
			cur_cmd->av = NULL;
			cur_cmd->redirs = NULL;
			cur_cmd->next = NULL;
		}
		else if (is_redir(token))
		{
			type = get_redir_type(token);
			filename = get_next_token(&data.line, env, &data);
			if (!filename)
			{
				ft_putendl_fd("Minishell: syntax error near unexpected token `newline'",
					2);
				free(token);
				free_cmds(cmd);
				free(data.ctx);
				return (NULL);
			}
			add_redir_to_cmd(cur_cmd, type, filename);
			free(filename);
			free(token);
		}
		else
		{
			cur_cmd->av = add_to_argv(cur_cmd->av, token);
			free(token);
		}
	}
	if (!cmd)
	{
		free(data.ctx);
		return (NULL);
	}
	if (!cur_cmd->av || cur_cmd->av[0] == NULL)
	{
		ft_putendl_fd("Minishell: syntax error near unexpected token `|'", 2);
		free_cmds(cmd);
		free(data.ctx);
		return (NULL);
	}
	free(data.ctx);
	return (cmd);
}
