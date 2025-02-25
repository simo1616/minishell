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

void	handle_heredoc(char *delimiter)
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
		write (pipe_fd[1], line, ft_strlen(line));
		write (pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		perror("dup2 heredoc");
	close(pipe_fd[0]);
}

void	handle_redirections(t_cmd *cmd)
{
	int		fd;
	t_redir	*redir;

	if (!cmd || !cmd->redirs)
		return ;
	redir = cmd->redirs;
	while (redir)
	{
		fd = -1;
		if (redir->type == REDIR_HEREDOC)
		{
			handle_heredoc(redir->filename);
			fd = STDIN_FILENO;
		}
		else if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1 && redir->type != REDIR_HEREDOC)
		{
			perror("open");
			return ;
		}
		if (fd != 1 && redir->type != REDIR_HEREDOC)
		{
			if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
				dup2(fd, STDIN_FILENO);
			else
				dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
}
