/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:15:51 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/28 21:12:25 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signal(t_shell_env *shell_env)
{
	if (g_received_signal)
	{
		if (g_received_signal == SIGINT)
			shell_env->exit_status = 130;
		g_received_signal = 0;
	}
}

static void	process_iteration(t_shell_env *shell_env)
{
	char		*cmd_line;
	t_cmd		*cmds;

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
	if (cmds == NULL)
		shell_env->exit_status = 258;
	else if (cmds->next)
	{
		exec_pipes(cmds, shell_env);
		free_cmds(cmds);
	}
	else
	{
		execute_commands(cmds, shell_env);
		free_cmds(cmds);
	}
	free(cmd_line);
}

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
