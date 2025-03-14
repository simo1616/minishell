/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:04:37 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 17:04:35 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Gère le caractère d'échappement.
 *
 * Si le caractère suivant existe, avance et remplit le buffer;
 * sinon, remplit le buffer et avance.
 *
 * @param tok Pointeur sur le tokenizer.
 */
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

/**
 * @brief Gère les guillemets dans le token.
 *
 * Active ou désactive le mode citation selon le caractère lu.
 *
 * @param tok Pointeur sur le tokenizer.
 */
static void	handle_quote_char(t_tokenizer *tok)
{
	char	c;

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

/**
 * @brief Parse un token depuis la ligne.
 *
 * Remplit le buffer et le contexte du token jusqu'à la fin ou
 * jusqu'à un espace hors citation.
 *
 * @param tok Pointeur sur le tokenizer.
 */
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

/**
 * @brief Initialise le tokenizer.
 *
 * Affecte les champs et saute les espaces initiaux.
 *
 * @param tok Pointeur sur le tokenizer.
 * @param env Environnement du shell.
 * @param data Structure contenant la ligne.
 */
static void	init_tokenizer(t_tokenizer *tok, t_shell_env *env, t_data *data)
{
	tok->data = data;
	tok->env = env;
	tok->pos = 0;
	tok->len = 0;
	tok->quote = 0;
	tok->ctx_start = data->cpos;
	skip_spaces(tok);
	data->line += tok->pos;
	data->cpos += tok->pos;
	tok->pos = 0;
	tok->total = ft_strlen(data->line);
}

/**
 * @brief Récupère le prochain token.
 *
 * Initialise le tokenizer, alloue le buffer, parse le token et
 * traite l'expansion si nécessaire.
 *
 * @param env Environnement du shell.
 * @param data Structure contenant la ligne.
 * @return t_token_data Token obtenu.
 */
t_token_data	get_next_token(t_shell_env *env, t_data *data)
{
	t_tokenizer		tok;
	t_token_data	result;

	result.token = NULL;
	result.token_ctx = NULL;
	result.len = 0;
	init_tokenizer(&tok, env, data);
	if (!tok.total || !alloc_buffer(&tok))
		return (result);
	parse_token(&tok);
	data->line += tok.pos;
	data->cpos += tok.pos;
	result.token = expand_token(&tok);
	result.token_ctx = tok.token_ctx;
	result.len = tok.len;
	return (result);
}
