/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:04:37 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/28 21:12:23 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_backslash(t_tokenizer *tok)
{
	if (tok->data->line[tok->pos + 1])
	{
		tok->pos++;
		fill_buffer_and_ctx(tok);
		tok->pos++;
	}
	else
	{
		fill_buffer_and_ctx(tok);
		tok->pos++;
	}
}

static void	handle_quote_char(t_tokenizer *tok)
{
    char c;

    c = tok->data->line[tok->pos];
    if (!tok->quote)
    {
        if (c == '\'')
            tok->quote = 1;
        else if (c == '\"')
            tok->quote = 2;
    }
    else if ((tok->quote == 1 && c == '\'') || (tok->quote == 2 && c == '"'))
        tok->quote = 0;
    else
        fill_buffer_and_ctx(tok);
    tok->pos++;
}

static void	parse_token(t_tokenizer *tok)
{
	while (tok->pos < tok->total && tok->data->line[tok->pos])
	{
		if (tok->data->line[tok->pos] == '\\')
			handle_backslash(tok);
		else if (tok->data->line[tok->pos] == '\''
			|| tok->data->line[tok->pos] == '"')
		{
			handle_quote_char(tok);
		}
		else
		{
			if (!tok->quote && ft_isspace(tok->data->line[tok->pos]))
				break ;
			fill_buffer_and_ctx(tok);
			tok->pos++;
		}
	}
	tok->buffer[tok->len] = '\0';
}

static void	skip_spaces(t_tokenizer *tok)
{
	while (tok->data->line[tok->pos] && ft_isspace(tok->data->line[tok->pos]))
		tok->pos++;
}

t_token_data get_next_token(t_shell_env *env, t_data *data)
{
    t_tokenizer		tok;
    t_token_data	result;

    result.token = NULL;
	result.token_ctx = NULL;
	result.len = 0;
	tok.data = data;
    tok.env = env;
    tok.pos = 0;
    tok.len = 0;
    tok.quote = 0;
    tok.ctx_start = data->cpos;

    skip_spaces(&tok);
    data->line += tok.pos;
    data->cpos += tok.pos;
    tok.pos = 0;
    tok.total = ft_strlen(data->line);
    if (!tok.total)
        return result;
    if (!alloc_buffer(&tok))
        return result;
    parse_token(&tok);
    data->line += tok.pos;
    data->cpos += tok.pos;
    result.token = expand_token(&tok);
    result.token_ctx = tok.token_ctx;
    result.len = tok.len;
    return result;
}
