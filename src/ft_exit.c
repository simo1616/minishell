/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:43:22 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/12 10:43:22 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	exit_args(char **args, long *exit_code, int *error, t_shell_env *shell)
{
	if (!ft_is_numeric(args[1]))
	{
		print_exit_error(args[1], "numeric argument required");
		exit(255);
	}
	*exit_code = ft_atol(args[1], error);
	if (*error)
	{
		print_exit_error(args[1], "numeric argument required");
		exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

int	ft_exit(char **args, t_shell_env *shell)
{
	long	exit_code;
	int		error;

	error = 0;
	ft_putstr_fd("exit\n", 1);
	if (!args[1])
		exit(shell->exit_status);
	if (exit_args(args, &exit_code, &error, shell) == 1)
		return (1);
	exit(exit_code % 256);
}
