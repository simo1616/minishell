/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 07:57:19 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/19 08:03:21 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_buffer_with_ctx(t_tokenizer *tok, int ctx_value)
{
	tok->buffer[tok->len] = tok->data->line[tok->pos];
	tok->token_ctx[tok->len] = ctx_value;
	tok->len++;
}

static void	handle_backslash_inside_quotes(t_tokenizer *tok)
{
	char	next;

	next = tok->data->line[tok->pos + 1];
	if (next == '\\' || next == '"' || next == '$' || next == '`')
	{
		tok->pos++;
		fill_buffer_with_ctx(tok, 1);
		tok->pos++;
	}
	else
	{
		fill_buffer_and_ctx(tok);
		tok->pos++;
	}
}

/**
 * @brief Gère le caractère d'échappement.
 *
 * Si le caractère suivant existe, avance et remplit le buffer;
 * sinon, remplit le buffer et avance.
 *
 * @param tok Pointeur sur le tokenizer.
 */

void	handle_backslash(t_tokenizer *tok)
{
	if (tok->quote == 1)
	{
		fill_buffer_and_ctx(tok);
		tok->pos++;
		return ;
	}
	if (tok->quote == 2)
	{
		handle_backslash_inside_quotes(tok);
		return ;
	}
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
