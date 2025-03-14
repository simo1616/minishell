/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcul_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 09:52:34 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 21:24:24 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calcule la longueur de la variable exit.
 *
 * Convertit le code de sortie en chaîne, calcule sa longueur,
 * libère la chaîne et retourne cette longueur.
 *
 * @param env Environnement du shell.
 * @return size_t Longueur de la chaîne de code de sortie.
 */
static size_t	process_exit_variable(t_shell_env *env)
{
	size_t	len;
	char	*exit_str;

	len = 0;
	exit_str = ft_itoa(env->exit_status);
	len = ft_strlen(exit_str);
	free(exit_str);
	return (len);
}

/**
 * @brief Calcule la longueur d'une variable normale.
 *
 * Extrait le nom de variable depuis token (à partir de l'indice *i),
 * récupère sa valeur dans env et retourne la longueur de cette valeur.
 * Met à jour l'indice *i.
 *
 * @param token Chaîne contenant la variable.
 * @param env Environnement du shell.
 * @param i Pointeur sur l'indice courant.
 * @return size_t Longueur de la valeur de la variable.
 */
static size_t	process_normal_variable(char *token, t_shell_env *env, int *i)
{
	size_t	len;
	char	*var_name;
	char	*var_value;
	int		var_start;
	int		var_len;

	len = 0;
	var_start = *i;
	while (token[*i] && is_valid_var_char(token[*i]))
		(*i)++;
	var_len = *i - var_start;
	var_name = ft_substr(token, var_start, var_len);
	var_value = env_get(env, var_name);
	if (var_value)
		len = ft_strlen(var_value);
	free(var_name);
	return (len);
}

/**
 * @brief Traite une variable dans token.
 *
 * Si le caractère courant est '?' traite le code de sortie,
 * sinon traite une variable normale ou retourne 2 pour un cas particulier.
 *
 * @param token Chaîne contenant la variable.
 * @param env Environnement du shell.
 * @param i Pointeur sur l'indice courant.
 * @return size_t Longueur calculée pour la variable.
 */
static size_t	process_variable(char *token, t_shell_env *env, int *i)
{
	size_t	len;

	len = 0;
	if (!token[*i])
		return (0);
	if (token[*i] == '?')
	{
		len = process_exit_variable(env);
		(*i)++;
	}
	else if (is_valid_var_char(token[*i]))
	{
		len = process_normal_variable(token, env, i);
	}
	else
	{
		len = 2;
		(*i)++;
	}
	return (len);
}

/**
 * @brief Gère les caractères spéciaux dans token.
 *
 * Si le caractère est une quote, ajoute 1.
 * Si c'est '$' et non cité, traite la variable.
 * Sinon, incrémente la longueur et l'indice.
 *
 * @param token Chaîne à traiter.
 * @param ctx Contexte de quotes.
 * @param env Environnement du shell.
 * @param i Pointeur sur l'indice courant.
 * @return size_t Longueur ajoutée pour ce caractère.
 */
static size_t	handle_special_chars(char *token, int *ctx, t_shell_env *env,
		int *i)
{
	size_t	len;

	len = 0;
	if (token[*i] == '\'' || token[*i] == '"')
	{
		len++;
		(*i)++;
	}
	else if (token[*i] == '$' && ctx[*i] != 1)
	{
		(*i)++;
		if (!token[*i])
			return (len);
		len += process_variable(token, env, i);
	}
	else
	{
		len++;
		(*i)++;
	}
	return (len);
}

/**
 * @brief Calcule la longueur finale d'un token.
 *
 * Parcourt token et, pour chaque caractère, ajoute la longueur
 * correspondante après traitement des variables et quotes.
 *
 * @param token Chaîne à traiter.
 * @param env Environnement du shell.
 * @param ctx Contexte de quotes.
 * @return size_t Longueur totale après expansion.
 */
size_t	calculate_length(char *token, t_shell_env *env, int *ctx)
{
	int		i;
	size_t	len;

	if (!ctx)
		return ((size_t)(-1));
	i = 0;
	len = 0;
	while (token[i])
		len += handle_special_chars(token, ctx, env, &i);
	return (len);
}
