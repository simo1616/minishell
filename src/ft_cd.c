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

	path = get_cd_path(args, shell_env);
	if (!path || check_cd_args(args, shell_env) == -1)
		return (1);
	if (validate_cd_path(path, shell_env) == -1)
		return (1);
	if (chdir(path) != 0)
		return (print_cd_error(path, "No such file or directory", shell_env));
	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
		update_env_pwd(shell_env, new_pwd);
	shell_env->exit_status = 0;
	return (0);
}
