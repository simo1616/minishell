#include "minishell.h"

char	*get_next_token(char **str, t_shell_env *env, t_data *data)
{
	char	*buffer;
	int		pos;
	int		len;
	int		quote;
	int		total;
	int		*old_ctx;
	size_t	old_cpos;
	int		*token_ctx;
	char	*token;

	pos = 0;
	while ((*str)[pos] && ft_isspace((*str)[pos]))
		pos++;
	*str += pos;
	pos = 0;
	len = 0;
	quote = 0;
	total = ft_strlen(*str);
	if (!total)
		return (NULL);
	buffer = malloc(sizeof(char) * (total + 1));
	token_ctx = malloc(sizeof(int) * (total + 1));
	if (!buffer || !token_ctx)
	{
		if (buffer)
			free(buffer);
		if (token_ctx)
			free(token_ctx);
		return (NULL);
	}
	while (pos < total && (*str)[pos])
	{
		if ((*str)[pos] == '\\')
		{
			if ((*str)[pos + 1])
			{
				pos++;
				buffer[len] = (*str)[pos];
				token_ctx[len] = quote;
				len++;
				pos++;
			}
			else
			{
				buffer[len] = (*str)[pos];
				token_ctx[len] = quote;
				len++;
				pos++;
			}
		}
		else if ((*str)[pos] == '\'' || (*str)[pos] == '"')
		{
			if (!quote)
			{
				if ((*str)[pos] == '\'')
					quote = 1;
				else if ((*str)[pos] == '\"')
					quote = 2;
			}
			else if ((quote == 1 && (*str)[pos] == '\'') || (quote == 2
					&& (*str)[pos] == '"'))
			{
				quote = 0;
			}
			else
			{
				buffer[len] = (*str)[pos];
				token_ctx[len] = quote;
				len++;
			}
			pos++;
		}
		else
		{
			if (!quote && ft_isspace((*str)[pos]))
				break ;
			buffer[len] = (*str)[pos];
			token_ctx[len] = quote;
			len++;
			pos++;
		}
	}
	buffer[len] = '\0';
	*str += pos;
	if (ft_strchr(buffer, '$'))
	{
		old_ctx = data->ctx;
		old_cpos = data->cpos;
		data->ctx = token_ctx;
		data->cpos = 0;
		token = remplacer_var(buffer, env, data);
		free(buffer);
		free(token_ctx);
		data->ctx = old_ctx;
		data->cpos = old_cpos;
	}
	else
	{
		token = buffer;
		free(token_ctx);
	}
	return (token);
}
