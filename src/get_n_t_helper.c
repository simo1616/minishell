/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_n_t_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:30:19 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/28 17:20:06 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	alloc_buffer(t_tokenizer *tok)
{
	tok->buffer = malloc(sizeof(char) * (tok->total + 1));
	tok->token_ctx = malloc(sizeof(int) * (tok->total + 1));
	if (!tok->buffer || !tok->token_ctx)
	{
		if (tok->buffer)
			free(tok->buffer);
		if (tok->token_ctx)
			free(tok->token_ctx);
		return (0);
	}
	return (1);
}

void	fill_buffer_and_ctx(t_tokenizer *tok)
{
	tok->buffer[tok->len] = tok->data->line[tok->pos];
	tok->token_ctx[tok->len] = tok->quote;
	tok->len++;
}

char *expand_token(t_tokenizer *tok)
{
	char *token;
	int *old_ctx;
	size_t old_cpos;

	if (!ft_strchr(tok->buffer, '$'))
	{
		token = tok->buffer;
		return (token);
	}
	old_ctx = tok->data->ctx;
	old_cpos = tok->data->cpos;
	tok->data->ctx = tok->token_ctx;
	tok->data->cpos = 0;
	token = remplacer_var(tok->buffer, tok->env, tok->data);
	free(tok->buffer);
	tok->data->ctx = old_ctx;
	tok->data->cpos = old_cpos;
	return (token);
}