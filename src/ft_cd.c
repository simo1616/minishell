/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:32:41 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/10 17:32:41 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **args, t_shell_env *shell_env)
{
	char		*path;
	char		new_pwd[PATH_MAX];
	struct stat	path_stat;

	if (!args[1] || strcmp(args[1], "~") == 0 || strcmp(args[1], "") == 0)
		path = env_get(shell_env, "HOME");
	else
		path = args[1];
	if (args[1] && args[2])
	{
		ft_putstr_fd("-minishell: cd: too many arguments\n", 2);
		shell_env->exit_status = 1;
		return (1);
	}
	if (stat(path, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
		shell_env->exit_status = 1;
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("-minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell_env->exit_status = 1;
		return (1);
	}
	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
	{
		env_set(shell_env, "OLDPWD", env_get(shell_env, "PWD"));
		env_set(shell_env, "PWD", new_pwd);
	}
	shell_env->exit_status = 0;
	return (0);
}
