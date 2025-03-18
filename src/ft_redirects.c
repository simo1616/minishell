/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:08:28 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/21 09:08:28 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_input(int write_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		if (g_received_signal)
			return (-1);
		line = readline("> ");
		if (!line || g_received_signal || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, int *heredoc_fd)
{
	int					pipe_fd[2];
	struct sigaction	sa;
	int					status;

	setup_heredoc_signal(&sa);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	status = read_heredoc_input(pipe_fd[1], delimiter);
	close(pipe_fd[1]);
	if (status == -1 || g_received_signal)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	*heredoc_fd = pipe_fd[0];
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	return (0);
}

/**
 * @brief Applique une redirection.
 *
 * Duplique le FD dans STDIN ou STDOUT selon le type, puis ferme FD.
 *
 * @param fd FD à rediriger.
 * @param type Type de redirection.
 */
static void	apply_redirection(int fd, int type)
{
	if (type == REDIR_IN || type == REDIR_HEREDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
}

/**
 * @brief Gère toutes les redirections d'une commande.
 *
 * Parcourt la liste des redirections et les applique.
 *
 * @param cmd Commande contenant les redirections.
 * @return int 0 si succès, -1 en cas d'erreur.
 */
int	handle_redirections(t_cmd *cmd)
{
	t_redir	*redir;
	int		heredoc_fd;
	int		fd;

	if (!cmd || !cmd->redirs)
		return (0);
	redir = cmd->redirs;
	heredoc_fd = -1;
	while (redir)
	{
		fd = open_redirection(redir, &heredoc_fd);
		if (fd == -1)
		{
			if (g_received_signal)
			{
				g_received_signal = 0;
				return (-1);
			}
			return (-1);
		}
		if (fd != -1)
			apply_redirection(fd, redir->type);
		redir = redir->next;
	}
	return (0);
}
