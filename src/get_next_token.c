#include "minishell.h"

static void	parse_token(t_tokenizer *tok)
{
		char	*buffer;
		int		pos;
		int		len;
		int		quote;
		int		total;
		int		*old_ctx;
		size_t	old_cpos;
		int		*token_ctx;
		int 	**dataline;

	while (tok->pos < tok->total && tok->data->line[tok->pos])
	{
		if (tok->data->line[tok->pos] == '\\')
		{
			if (tok->data->line[tok->pos + 1])
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
		else if (tok->data->line[tok->pos] == '\'' || tok->data->line[tok->pos] == '"')
		{
			if (!tok->quote)
			{
				if ((*str)[pos] == '\'')
					quote = 1;
				else if ((*str)[pos] == '\"')
					quote = 2;
			}
			else if ((quote == 1 && (*str)[pos] == '\'') || (quote == 2
					&& (*str)[pos] == '"'))
			{
				tok->quote = 0;
			}
			else
			{
				buffer[len] = (*str)[pos];
				token_ctx[len] = quote;
				len++;
			}
			tok->pos++;
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
