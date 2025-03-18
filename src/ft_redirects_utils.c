/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:50:30 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/18 16:50:30 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler_heredoc(int signo)
{
	(void)signo;
	g_received_signal = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	setup_heredoc_signal(struct sigaction *sa)
{
	sa->sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);
}

static int	handle_file_redir(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		perror(redir->filename);
	return (fd);
}

static int	handle_heredoc_redir(t_redir *redir, int *heredoc_fd)
{
	if (handle_heredoc(redir->filename, heredoc_fd) == -1)
	{
		g_received_signal = 0;
		return (-1);
	}
	return (*heredoc_fd);
}

int	open_redirection(t_redir *redir, int *heredoc_fd)
{
	if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc_redir(redir, heredoc_fd));
	return (handle_file_redir(redir));
}
