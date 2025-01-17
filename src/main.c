/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:26:49 by mbendidi          #+#    #+#             */
/*   Updated: 2025/01/17 15:22:34 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_cmd(char *cmd)
{
	char	**args;
	int		i;
	char	*token;

	args = malloc(sizeof(char *) * 64);
	i = 0;
	token = ft_strtok(cmd, " \n");
	while (token != NULL && i < 63)
	{
		args[i] = ft_strdup(token);
		token = ft_strtok(NULL, " \n");
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

int	main(void)
{
	char *cmd;
	char **args;
	pid_t pid;

	while (1)
	{
		write(1, "minishell> ", 11);
		cmd = get_next_line(0);
		if (!cmd)
			break ;
		if (cmd[0] == '\n')
		{
			free(cmd);
			continue ;
		}

		args = parse_cmd(cmd);
		free(cmd);
		if (!args[0])
		{
			free_args(args);
			continue ;
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			free_args(args);
			continue ;
		}

		if (pid == 0)
		{
			execvp(args[0], args);
			perror("exec failed");
			free_args(args);
			exit(1);
		}
		else
			wait(NULL);

		free_args(args);
	}
	write(1, "\nExit minishell\n", 15);
	return (0);
}