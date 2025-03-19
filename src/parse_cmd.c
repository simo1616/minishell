/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:50:28 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/18 23:01:02 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise une commande si nécessaire.
 *
 * Si la commande n'est pas encore allouée, alloue une nouvelle structure
 * t_cmd, initialise ses champs et met à jour cur_cmd.
 *
 * @param cmd Pointeur vers la commande principale.
 * @param cur_cmd Pointeur vers la commande courante.
 * @return int 1 en cas de succès, 0 en cas d'échec.
 */
int	init_cmd_ifneed(t_cmd **cmd, t_cmd **cur_cmd)
{
	if (!*cmd)
	{
		*cmd = malloc(sizeof(t_cmd));
		if (!*cmd)
			return (0);
		*cur_cmd = *cmd;
		(*cur_cmd)->next = NULL;
		(*cur_cmd)->av = NULL;
		(*cur_cmd)->redirs = NULL;
	}
	return (1);
}

/**
 * @brief Parse les tokens de la ligne de commande en une liste.
 *
 * Lit et traite les tokens jusqu'à épuisement. Pour chaque token, 
 * initialise la commande si nécessaire et appelle process_token.
 * En cas d'erreur, libère les commandes et retourne NULL.
 *
 * @param data Structure contenant la ligne de commande et son contexte.
 * @param env Environnement du shell.
 * @return t_cmd* Liste chaînée des commandes ou NULL en cas d'erreur.
 */
t_cmd	*parse_tokens(t_data *data, t_shell_env *env)
{
	t_cmd			*cmd;
	t_cmd			*cur_cmd;
	t_token_data	token_data;

	cmd = NULL;
	cur_cmd = NULL;
	while (1)
	{
		token_data = get_next_token(env, data);
		if (!token_data.token)
		{
			if (token_data.token_ctx)
				free(token_data.token_ctx);
			break ;
		}
		if (!init_cmd_ifneed(&cmd, &cur_cmd) || !process_token(token_data, data,
				env, &cur_cmd))
		{
			free_cmds(cmd);
			free(token_data.token_ctx);
			return (NULL);
		}
		free(token_data.token_ctx);
	}
	return (cmd);
}

/**
 * @brief Vérifie la syntaxe finale de la liste de commandes.
 *
 * Parcourt la liste jusqu'à la dernière commande et vérifie que celle-ci 
 * possède des arguments. Si non, affiche une erreur, libère les commandes,
 * et retourne 0.
 *
 * @param cmd Liste chaînée des commandes.
 * @return int 1 si la vérification est réussie, 0 sinon.
 */
int	final_verification(t_cmd *cmd)
{
	t_cmd	*cur_cmd;

	cur_cmd = cmd;
	while (cur_cmd->next)
		cur_cmd = cur_cmd->next;
	if (!cur_cmd->av || cur_cmd->av[0] == NULL)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		free_cmds(cmd);
		return (0);
	}
	return (1);
}

/**
 * @brief Parse la ligne de commande et retourne la liste de commandes.
 *
 * Initialise la structure t_data avec la ligne, récupère le contexte des 
 * citations, parse les tokens en commandes et effectue une vérification finale.
 * En cas d'erreur, libère le contexte et retourne NULL.
 *
 * @param line Ligne de commande saisie.
 * @param env Environnement du shell.
 * @return t_cmd* Liste chaînée des commandes ou NULL en cas d'erreur.
 */
t_cmd	*parse_command_line(char *line, t_shell_env *env)
{
	t_data	data;
	t_cmd	*cmd;

	if (!line)
		return (NULL);
	data.line = line;
	data.cpos = 0;
	if (!get_quotes_context(&data))
		return (NULL);
	cmd = parse_tokens(&data, env);
	if (!cmd)
	{
		free(data.ctx);
		return (NULL);
	}
	if (!final_verification(cmd))
	{
		free(data.ctx);
		return (NULL);
	}
	free(data.ctx);
	return (cmd);
}
