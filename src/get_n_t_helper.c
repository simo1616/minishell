/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_n_t_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:30:19 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/19 08:01:31 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Saute les espaces dans la ligne.
 *
 * Avance la position tant que des espaces sont rencontrés.
 *
 * @param tok Pointeur sur le tokenizer.
 */
void	skip_spaces(t_tokenizer *tok)
{
	while (tok->data->line[tok->pos] && ft_isspace(tok->data->line[tok->pos]))
		tok->pos++;
}

/**
 * @brief Alloue le buffer et le contexte du token.
 *
 * Alloue la mémoire pour le buffer et le tableau de contexte,
 * selon la taille totale de la ligne.
 *
 * @param tok Pointeur sur le tokenizer.
 * @return int 1 si réussi, 0 sinon.
 */
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

/**
 * @brief Ajoute le caractère courant au buffer.
 *
 * Copie le caractère et le mode citation dans le contexte.
 *
 * @param tok Pointeur sur le tokenizer.
 */
void	fill_buffer_and_ctx(t_tokenizer *tok)
{
	tok->buffer[tok->len] = tok->data->line[tok->pos];
	tok->token_ctx[tok->len] = tok->quote;
	tok->len++;
}

/**
 * @brief Étend le token si des variables y sont présentes.
 *
 * Si le buffer contient '$', appelle la fonction de remplacement,
 * sinon retourne le buffer.
 *
 * @param tok Pointeur sur le tokenizer.
 * @return char* Token final.
 */
char	*expand_token(t_tokenizer *tok)
{
	char	*token;
	int		*old_ctx;
	size_t	old_cpos;

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
	if (!token)
	{
		tok->data->ctx = old_ctx;
		tok->data->cpos = old_cpos;
		return (NULL);
	}
	tok->data->ctx = old_ctx;
	tok->data->cpos = old_cpos;
	return (token);
}
