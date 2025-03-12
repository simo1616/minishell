/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:39:15 by jdecarro          #+#    #+#             */
/*   Updated: 2025/03/11 15:39:15 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cd_path(char **args, t_shell_env *shell_env)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0 || ft_strcmp(args[1], "") == 0)
		return (env_get(shell_env, "HOME"));
	return (args[1]);
}

int	check_cd_args(char **args, t_shell_env *shell_env)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell_env->exit_status = 1;
		return (-1);
	}
	return (0);
}

int	validate_cd_path(char *path, t_shell_env *shell_env)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode))
		return (print_cd_error(path, "Not a directory", shell_env));
	return (0);
}

int	print_cd_error(char *path, char *error_msg, t_shell_env *shell_env)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\n", 2);
	shell_env->exit_status = 1;
	return (1);
}

void	update_env_pwd(t_shell_env *shell_env, char *new_pwd)
{
	env_set(shell_env, "OLDPWD", env_get(shell_env, "PWD"));
	env_set(shell_env, "PWD", new_pwd);
}
