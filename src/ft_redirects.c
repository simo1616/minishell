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

/**
 * @brief Gère un heredoc.
 *
 * Crée un pipe, lit les lignes avec "> " et écrit dans le pipe
 * jusqu'à ce que le délimiteur soit atteint.
 *
 * @param delimiter Chaîne indiquant la fin du heredoc.
 * @param heredoc_fd Pointeur sur le FD heredoc.
 */
static void	handle_heredoc(char *delimiter, int *heredoc_fd)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	*heredoc_fd = pipe_fd[0];
}

/**
 * @brief Ouvre un FD selon le type de redirection.
 *
 * Pour un heredoc, appelle handle_heredoc. Pour une
 * redirection d'entrée, ouvre le fichier en lecture.
 * Pour la sortie, ouvre en écriture (troncature ou ajout).
 *
 * @param redir Redirection à traiter.
 * @param heredoc_fd Pointeur sur le FD heredoc.
 * @return int FD ouvert ou -1 en cas d'erreur.
 */
static int	open_redirection(t_redir *redir, int *heredoc_fd)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_HEREDOC)
	{
		handle_heredoc(redir->filename, heredoc_fd);
		fd = *heredoc_fd;
	}
	else if (redir->type == REDIR_IN)
	{
		fd = open(redir->filename, O_RDONLY);
		if (fd == -1)
		{
			perror(redir->filename);
			return (-1);
		}
	}
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	return (fd);
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
		if (fd == -1 && redir->type != REDIR_HEREDOC)
			return (-1);
		if (fd != -1)
			apply_redirection(fd, redir->type);
		redir = redir->next;
	}
	return (0);
}
