/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:49:19 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 15:52:26 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Seule variable globale autorisée pour stocker le signal reçu */
int			g_received_signal = 0;

/**
 * @brief Fonction de gestion de signal.
 *
 * Cette fonction est appelée automatiquement lorsqu'un signal est reçu.
 * Elle stocke le numéro du signal dans g_received_signal 
 * pour le traitement global.
 * Si le signal est SIGINT (généré par Ctrl+C), 
 * elle effectue les opérations suivantes :
 *   - Affiche un saut de ligne afin de passer à une nouvelle ligne.
 *   - Informe readline qu'une nouvelle ligne débute avec rl_on_new_line().
 *   - Efface la ligne de commande en cours via rl_replace_line("", 0) pour 
 * éviter d'afficher 
 *     du texte partiellement saisi.
 *   - Redessine l'invite de commande avec rl_redisplay(), permettant à 
 * l'utilisateur de saisir 
 *     une nouvelle commande proprement.
 *
 * @param signum Le numéro du signal reçu (exemple : SIGINT).
 */
static void	signal_handler(int signum)
{
	g_received_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/**
 * @brief Configure la gestion des signaux du shell.
 *
 * Cette fonction associe le signal SIGINT à la fonction signal_handler, 
 * permettant de capturer et de traiter l'interruption (Ctrl+C).
 * De plus, elle ignore le signal SIGQUIT (généré par Ctrl+\) 
 * afin que le shell ne se termine pas ou ne se comporte pas de 
 * manière inattendue lors de sa réception.
 */
void	signal_setup(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
