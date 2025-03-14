/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:15:51 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 15:52:50 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Met à jour le statut de sortie en fonction du signal reçu.
 *
 * Si SIGINT est reçu, le statut devient 130, puis réinitialise le signal.
 *
 * @param shell_env Environnement du shell.
 */
static void	handle_signal(t_shell_env *shell_env)
{
	if (g_received_signal)
	{
		if (g_received_signal == SIGINT)
			shell_env->exit_status = 130;
		g_received_signal = 0;
	}
}

/**
 * @brief Exécute les commandes selon leur structure et libère la mémoire.
 *
 * Si cmds est NULL, le statut passe à 258 (sauf en cas de SIGINT).
 * Si plusieurs commandes sont chaînées (pipe), appelle exec_pipes(),
 * sinon exécute la commande avec execute_commands().
 *
 * @param cmds Structure des commandes.
 * @param shell_env Environnement du shell.
 */
static void	handle_command_execution(t_cmd *cmds, t_shell_env *shell_env)
{
	if (cmds == NULL)
	{
		if (shell_env->exit_status != 130)
			shell_env->exit_status = 258;
	}
	else if (cmds->next)
		exec_pipes(cmds, shell_env);
	else
		execute_commands(cmds, shell_env);
	free_cmds(cmds);
}

/**
 * @brief Lit la ligne de commande, la parse et exécute la commande.
 *
 * Lit la commande via readline(), ajoute à l'historique,
 * gère les signaux, parse la commande et exécute selon sa structure.
 *
 * @param shell_env Environnement du shell.
 */
static void	process_iteration(t_shell_env *shell_env)
{
	char	*cmd_line;
	t_cmd	*cmds;

	cmd_line = readline("\001\033[1;35m\002Minishell> \001\033[0m\002");
	if (!cmd_line)
	{
		printf("exit\n");
		shell_env->running = 0;
		return ;
	}
	if (*cmd_line)
		add_history(cmd_line);
	handle_signal(shell_env);
	cmds = parse_command_line(cmd_line, shell_env);
	handle_command_execution(cmds, shell_env);
	free(cmd_line);
}

/**
 * @brief Point d'entrée du shell.
 *
 * Initialise l'environnement et la gestion des signaux, entre dans 
 * la boucle d'invite.
 * 
 * Si des arguments supplémentaires sont fournis, affiche une erreur.
 *
 * @param ac Nombre d'arguments.
 * @param av Arguments.
 * @param envp Variables d'environnement.
 * @return int Code de sortie.
 */
int	main(int ac, char **av, char **envp)
{
	t_shell_env	*shell_env;
	int			ret;

	if (ac == 1)
	{
		shell_env = create_shell_env(envp);
		if (!shell_env)
			return (1);
		signal_setup();
		while (shell_env->running)
			process_iteration(shell_env);
		ret = shell_env->exit_status;
		destroy_shell_env(shell_env);
		return (ret);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		write(2, av[1], ft_strlen(av[1]));
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
}
