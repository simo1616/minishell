/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:41:26 by jdecarro          #+#    #+#             */
/*   Updated: 2025/01/31 09:41:26 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie l'option -n pour echo.
 *
 * Retourne true si la chaîne commence par '-' et contient
 * uniquement des 'n' après.
 *
 * @param str Chaîne à tester.
 * @return bool true si option -n, false sinon.
 */
static bool	check_option_echo(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (!str[i])
		return (false);
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

/**
 * @brief Affiche les arguments d'echo.
 *
 * Parcourt args à partir de l'indice i, écrit les mots
 * séparés par un espace, et ajoute un saut de ligne si
 * new_line est true.
 *
 * @param i Index de départ dans args.
 * @param new_line Indique s'il faut ajouter un saut de ligne.
 * @param args Tableau d'arguments.
 */
static void	write_echo(int i, bool new_line, char **args)
{
	bool	first;

	first = true;
	while (args[i])
	{
		if (!first)
			write(1, " ", 1);
		write(1, args[i], ft_strlen(args[i]));
		first = false;
		i++;
	}
	if (new_line)
		write(1, "\n", 1);
}

/**
 * @brief Exécute la commande echo.
 *
 * Gère l'option -n pour supprimer le saut de ligne.
 *
 * @param args Tableau d'arguments.
 * @param shell_env Environnement du shell (non utilisé).
 * @return int 0.
 */
int	ft_echo(char **args, t_shell_env *shell_env)
{
	int		i;
	bool	new_line;

	(void)shell_env;
	i = 1;
	new_line = true;
	while (args[i] && check_option_echo(args[i]))
	{
		new_line = false;
		i++;
	}
	write_echo(i, new_line, args);
	return (0);
}
