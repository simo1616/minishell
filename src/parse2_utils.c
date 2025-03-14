/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:01:13 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 20:13:57 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Met à jour le contexte de quote selon le caractère.
 *
 * Alterne l'état de quote simple si le caractère est '\'' et 
 * aucune quote double n'est active, et vice versa pour '"'.
 *
 * @param c Caractère courant.
 * @param quote Pointeur vers l'état de quote.
 */
static void	check_quote(char c, int *quote)
{
	if (c == '\'' && *quote != 2)
	{
		if (*quote == 0)
			*quote = 1;
		else
			*quote = 0;
	}
	else if (c == '"' && *quote != 1)
	{
		if (*quote == 0)
			*quote = 2;
		else
			*quote = 0;
	}
}

/**
 * @brief Remplit le contexte de quotes pour chaque caractère.
 *
 * Parcourt la ligne et stocke l'état de quote dans le tableau.
 *
 * @param line Ligne de commande.
 * @param context Tableau pour stocker le contexte.
 * @return int Dernier état de quote.
 */
static int	remplir_quotes_context(const char *line, int *context)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		check_quote(line[i], &quote);
		context[i++] = quote;
	}
	context[i] = 0;
	return (quote);
}

/**
 * @brief Récupère le contexte de quotes de la ligne.
 *
 * Alloue et remplit le tableau de contexte.
 * Si une quote n'est pas fermée, affiche une erreur et retourne NULL.
 *
 * @param data Structure contenant la ligne.
 * @return void* Pointeur sur data ou NULL en cas d'erreur.
 */
void	*get_quotes_context(t_data *data)
{
	if (!data->line)
		return (NULL);
	data->ctx = malloc(sizeof(int) * (ft_strlen(data->line) + 1));
	if (!data->ctx)
		return (NULL);
	if (remplir_quotes_context(data->line, data->ctx) != 0)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		free(data->ctx);
		return (NULL);
	}
	return (data);
}

/**
 * @brief Crée un nouveau tableau d'arguments.
 *
 * Alloue un tableau de taille len + 2 et copie les anciens
 * arguments dans le nouveau tableau.
 *
 * @param av Tableau d'arguments existant.
 * @param len Nombre d'arguments dans av.
 * @return char** Nouveau tableau d'arguments.
 */
static char	**create_new_argv(char **av, int len)
{
	char	**new_av;
	int		i;

	new_av = malloc(sizeof(char *) * (len + 2));
	if (!new_av)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_av[i] = av[i];
		i++;
	}
	return (new_av);
}

/**
 * @brief Ajoute un token au tableau d'arguments.
 *
 * Si av est vide, initialise un tableau avec token.
 * Sinon, alloue un nouveau tableau, ajoute token et libère l'ancien.
 *
 * @param av Tableau d'arguments.
 * @param token Chaîne à ajouter.
 * @return char** Nouveau tableau d'arguments.
 */
char	**add_to_argv(char **av, const char *token)
{
	char	**new_av;
	int		len;

	if (!av || !av[0])
		return (init_argv(token));
	len = 0;
	while (av[len])
		len++;
	new_av = create_new_argv(av, len);
	if (!new_av)
		return (NULL);
	new_av[len] = ft_strdup(token);
	new_av[len + 1] = NULL;
	free(av);
	return (new_av);
}
