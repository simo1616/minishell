/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:35:10 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/14 15:35:10 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Copie le tableau d'environnement.
 *
 * Alloue et duplique chaque chaîne de env.
 *
 * @param env Tableau d'environnement.
 * @return char** Nouveau tableau ou NULL en erreur.
 */
static char	**ft_copy_env(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (i-- > 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/**
 * @brief Trie le tableau d'environnement.
 *
 * tri simple par comparaison des chaînes.
 *
 * @param env Tableau d'environnement à trier.
 */
static void	ft_sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Affiche l'environnement trié.
 *
 * Copie et trie l'env, puis l'affiche sous la forme
 * "declare -x name" ou "declare -x name="value"".
 *
 * @param env Tableau d'environnement.
 */
void	ft_print_sorted_env(char **env)
{
	int		i;
	int		j;
	char	**sorted_env;

	sorted_env = ft_copy_env(env);
	if (!sorted_env)
		return ;
	ft_sort_env(sorted_env);
	i = 0;
	while (sorted_env[i])
	{
		ft_printf("declare -x ");
		j = 0;
		while (sorted_env[i][j] && sorted_env[i][j] != '=')
			ft_printf("%c", sorted_env[i][j++]);
		if (sorted_env[i][j] && sorted_env[i][j] == '=')
			ft_printf("=\"%s\"\n", &sorted_env[i][j + 1]);
		else
			ft_printf("\n");
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

/**
 * @brief Vérifie si un nom pour export est valide.
 *
 * Checker le nom, ne doit pas commencer par '=' ou un chiffre, et doit
 * contenir uniquement des alphanumériques ou '_'.
 *
 * @param args Nom à vérifier.
 * @return int 1 si valide, 0 sinon.
 */
int	check_export_name(char *args)
{
	int	i;

	if (!args || !args[0] || args[0] == '=' || ft_isdigit(args[0]))
		return (0);
	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
