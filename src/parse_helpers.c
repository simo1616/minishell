/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:28:15 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 20:15:32 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si c est un caractère valide pour une variable.
 *
 * Retourne vrai si c est alphanumérique ou '_'.
 *
 * @param c Caractère à vérifier.
 * @return int 1 si valide, 0 sinon.
 */
int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * @brief Initialise le tableau d'arguments avec token.
 *
 * Alloue un tableau de 2 pointeurs : le premier contient une copie
 * de token, le second est NULL.
 *
 * @param token Chaîne à copier.
 * @return char** Tableau d'arguments ou NULL en cas d'erreur.
 */
char	**init_argv(const char *token)
{
	char	**new_av;

	new_av = malloc(sizeof(char *) * 2);
	if (!new_av)
		return (NULL);
	new_av[0] = ft_strdup(token);
	new_av[1] = NULL;
	return (new_av);
}

/**
 * @brief Remplace "$?" par le code de sortie dans v->new.
 *
 * Convertit le code de sortie en chaîne, le copie dans v->new, et
 * met à jour les index v->i et v->data->cpos.
 *
 * @param v Structure de variables pour le remplacement.
 */
void	handle_exit_status(t_var *v)
{
	char	*exit_str;

	exit_str = ft_itoa(v->env->exit_status);
	ft_memcpy(v->new + v->j, exit_str, ft_strlen(exit_str));
	v->j += ft_strlen(exit_str);
	v->i++;
	v->data->cpos++;
	free(exit_str);
}
