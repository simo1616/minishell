/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:39:21 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 20:17:35 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Gère le pipe dans une commande.
 *
 * Vérifie que la commande courante possède des arguments.
 * Alloue une nouvelle commande pour la suite.
 *
 * @param cur_cmd Pointeur vers la commande courante.
 * @return int 1 si succès, 0 sinon.
 */
int	handle_pipe(t_cmd **cur_cmd)
{
	if (!(*cur_cmd)->av || (*cur_cmd)->av[0] == NULL)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (0);
	}
	(*cur_cmd)->next = malloc(sizeof(t_cmd));
	if (!(*cur_cmd)->next)
		return (0);
	*cur_cmd = (*cur_cmd)->next;
	(*cur_cmd)->av = NULL;
	(*cur_cmd)->redirs = NULL;
	(*cur_cmd)->next = NULL;
	return (1);
}

/**
 * @brief Gère une redirection.
 *
 * Récupère le type de redirection et le nom du fichier.
 * Vérifie la validité du token suivant et l'ajoute
 * à la commande.
 *
 * @param token Token de redirection.
 * @param data Structure de données.
 * @param env Environnement du shell.
 * @param cur_cmd Pointeur vers la commande courante.
 * @return int 1 si succès, 0 sinon.
 */
int	handle_redir(char *token, t_data *data, t_shell_env *env, t_cmd **cur_cmd)
{
	t_redir_type	type;
	t_token_data	filename_data;
	char			*filename;

	type = get_redir_type(token);
	filename_data = get_next_token(env, data);
	if (!filename_data.token || is_redir(filename_data.token)
		|| !ft_strcmp(filename_data.token, "|"))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (filename_data.token)
			ft_putstr_fd(filename_data.token, 2);
		else
			ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
		if (filename_data.token)
			free(filename_data.token);
		return (0);
	}
	filename = filename_data.token;
	add_redir_to_cmd(*cur_cmd, type, filename);
	free(filename_data.token_ctx);
	return (1);
}

/**
 * @brief Vérifie si tous les indices du token sont cités.
 *
 * Parcourt le contexte de token pour s'assurer que
 * tous les caractères sont entre quotes.
 *
 * @param token_data Structure contenant le token et son contexte.
 * @return int 1 si entièrement cité, 0 sinon.
 */
static int	is_token_quoted(t_token_data token_data)
{
	size_t	i;

	i = 0;
	while (i < token_data.len)
	{
		if (token_data.token_ctx[i] == 0)
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Traite les tokens de redirection.
 *
 * Pour chaque token de redirection, appelle handle_redir.
 * Si un token est un pipe, gère le passage à la commande suivante.
 * Ajoute un argument si le token n'est ni redirection ni pipe.
 *
 * @param data Structure de données.
 * @param env Environnement du shell.
 * @param cur_cmd Pointeur vers la commande courante.
 * @param token_data Token courant.
 * @return int 1 si succès, 0 sinon.
 */
static int	process_token_redir(t_data *data, t_shell_env *env, t_cmd **cur_cmd,
		t_token_data token_data)
{
	int	res;

	while (token_data.token && is_redir(token_data.token))
	{
		res = handle_redir(token_data.token, data, env, cur_cmd);
		free(token_data.token);
		if (!res)
			return (0);
		token_data = get_next_token(env, data);
	}
	if (token_data.token && !ft_strcmp(token_data.token, "|"))
	{
		res = handle_pipe(cur_cmd);
		free(token_data.token);
		return (res);
	}
	if (token_data.token)
	{
		(*cur_cmd)->av = add_to_argv((*cur_cmd)->av, token_data.token);
		free(token_data.token);
	}
	return (1);
}

/**
 * @brief Traite un token de commande.
 *
 * Si le token est entièrement cité ou s'il n'est pas un pipe
 * et n'est pas une redirection, l'ajoute à la commande.
 * Sinon, s'il s'agit d'un pipe, gère la fin de commande,
 * sinon traite les redirections.
 *
 * @param token_data Token courant.
 * @param data Structure de données.
 * @param env Environnement du shell.
 * @param cur_cmd Pointeur vers la commande courante.
 * @return int 1 si succès, 0 sinon.
 */
int	process_token(t_token_data token_data, t_data *data, t_shell_env *env,
		t_cmd **cur_cmd)
{
	int	res;

	if (is_token_quoted(token_data) || (ft_strcmp(token_data.token, "|")
			&& !is_redir(token_data.token)))
	{
		(*cur_cmd)->av = add_to_argv((*cur_cmd)->av, token_data.token);
		free(token_data.token);
		return (1);
	}
	if (!ft_strcmp(token_data.token, "|"))
	{
		res = handle_pipe(cur_cmd);
		free(token_data.token);
		return (res);
	}
	return (process_token_redir(data, env, cur_cmd, token_data));
}
