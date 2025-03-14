/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:36:07 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/28 19:24:29 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child_process(char *path, t_cmd *cmd, t_shell_env *shell_env)
{
	signal(SIGQUIT, SIG_DFL);
	execve(path, cmd->av, shell_env->env);
	perror("minishell: error Child process\n");
	free(path);
	exit(EXIT_FAILURE);
}

static int	handle_no_path(char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (EX_CMD_NT_FD);
}

static int	handle_fork_error(char *path, t_shell_env *shell_env)
{
	perror("minishell");
	shell_env->exit_status = 1;
	free(path);
	return (shell_env->exit_status);
}

void	update_exit_status(t_shell_env *shell_env, int status)
{
	if (WIFEXITED(status))
		shell_env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell_env->exit_status = EXIT_SIG_OFFSET + WTERMSIG(status);
}

int	exec_external(t_cmd *cmd, t_shell_env *shell_env)
{
	pid_t		pid;
	char		*path;
	int			status;
	struct stat	sb;

	path = resolve_path(cmd->av[0], shell_env->env);
	if (!path)
		return (handle_no_path(cmd->av[0]));
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		free(path);
		return (EX_CMD_NT_EXE);
	}
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(path, shell_env));
	if (pid == 0)
		exec_child_process(path, cmd, shell_env);
	waitpid(pid, &status, 0);
	free(path);
	update_exit_status(shell_env, status);
	return (shell_env->exit_status);
}
