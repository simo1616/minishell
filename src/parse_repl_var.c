/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_repl_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:05:27 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 20:11:35 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Traite une variable valide.
 *
 * Extrait le nom de la variable depuis token, récupère sa valeur
 * depuis l'environnement et copie cette valeur dans la nouvelle
 * chaîne.
 *
 * @param v Structure de variables pour le remplacement.
 */
static void	handle_valid_var(t_var *v)
{
	char	*var_name;
	char	*var_value;
	int		var_start;

	var_start = v->i;
	while (v->token[v->i] && is_valid_var_char(v->token[v->i]))
	{
		v->i++;
		v->data->cpos++;
	}
	var_name = ft_substr(v->token, var_start, v->i - var_start);
	var_value = env_get(v->env, var_name);
	if (var_value)
	{
		ft_memcpy(v->new + v->j, var_value, ft_strlen(var_value));
		v->j += ft_strlen(var_value);
	}
	free(var_name);
}

/**
 * @brief Traite le caractère '$'.
 *
 * Avance l'index et selon le caractère suivant, traite le cas de
 * fin de chaîne, '?' ou d'un caractère non valide.
 *
 * @param v Structure de variables pour le remplacement.
 */
static void	handle_dollar(t_var *v)
{
	v->i++;
	v->data->cpos++;
	if (!v->token[v->i])
		v->new[v->j++] = '$';
	else if (v->token[v->i] == '?')
		handle_exit_status(v);
	else if (!is_valid_var_char(v->token[v->i]))
	{
		v->new[v->j++] = '$';
		v->new[v->j++] = v->token[v->i++];
		v->data->cpos++;
	}
	else
		handle_valid_var(v);
}

/**
 * @brief Traite le '$' dans une citation.
 *
 * Copie le caractère '$' et ses suivants tant que le contexte 
 * indique une citation (ctx == 1).
 *
 * @param v Structure de variables pour le remplacement.
 */
static void	process_quoted_dollar(t_var *v)
{
	while (v->token[v->i] && v->data->ctx[v->data->cpos] == 1)
	{
		v->new[v->j++] = v->token[v->i++];
		v->data->cpos++;
	}
}

/**
 * @brief Parcourt token et remplace les variables.
 *
 * Itère sur token et, selon le caractère, traite la variable ou
 * copie le caractère directement.
 *
 * @param v Structure de variables pour le remplacement.
 */
static void	process_characters(t_var *v)
{
	while (v->token[v->i])
	{
		if (v->token[v->i] == '$' && v->data->ctx[v->data->cpos] != 1)
			handle_dollar(v);
		else if (v->token[v->i] == '$')
			process_quoted_dollar(v);
		else
		{
			v->new[v->j++] = v->token[v->i++];
			v->data->cpos++;
		}
	}
}

/**
 * @brief Remplace les variables dans token par leur valeur.
 *
 * Alloue une nouvelle chaîne, parcourt token pour remplacer
 * les variables et retourne la nouvelle chaîne.
 *
 * @param token Chaîne contenant des variables.
 * @param env Environnement du shell.
 * @param data Structure des données de la ligne.
 * @return char* Chaîne avec les variables remplacées.
 */
char	*remplacer_var(char *token, t_shell_env *env, t_data *data)
{
	t_var	v;

	if (ft_strcmp(token, "$") == 0)
		return (ft_strdup(token));
	v.token = token;
	v.env = env;
	v.data = data;
	v.new = malloc(calculate_length(token, env, data->ctx) + 1);
	v.i = 0;
	v.j = 0;
	v.len = calculate_length(token, env, data->ctx);
	if (!v.new || !data->ctx)
	{
		if (v.new)
			free(v.new);
		return (NULL);
	}
	process_characters(&v);
	v.new[v.j] = '\0';
	return (v.new);
}
