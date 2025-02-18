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

/* void	update_env_vars(char *new_pwd)
{
	char	oldpwd[PATH_MAX];

	if (getcwd(oldpwd, sizeof(oldpwd) != NULL));
		setenv("OLDPWD", oldpwd, 1);
	setenv("PWD", new_pwd, 1);
}
void	ft_cd(char **args)
{
	char	*path;

	if (!args[1])
		path = getenv("HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
} */

/* void update_env_vars()
{
	char oldpwd[PATH_MAX];
	char newpwd[PATH_MAX];

	if (getcwd(newpwd, sizeof(newpwd)) == NULL)
	{
		perror("cd");
		return ;
	}
	if (getenv("PWD"))
		setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", newpwd, 1);  // Met à jour PWD avec le nouveau chemin
}

void ft_cd(char **args)
{
	char	*path;
	char	*oldpwd[PATH_MAX];

	if (getcwd(oldpwd, sizeof(oldpwd)) != NULL)
		setenv("OLDPWD", oldpwd, 1);
	if (!args[1])
		path = getenv("HOME");
	else if (strcmp(args[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return;
		}
		printf("%s\n", path);
	}
	else
		path = args[1];

	if (!path)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return;
	}

	if (chdir(path) != 0)
	{
		perror("cd");
		return;
	}
	update_env_vars();
} */

int	ft_cd(char **args, t_shell_env *shell_env)
{
	char	new_pwd[PATH_MAX];
	char	*oldpwd;

	oldpwd = env_get(shell_env, "PWD");
	if (!args[1])
	{
		ft_putstr_fd("cd : missing argument\n", 2);
		shell_env->exit_status = 1;
		return (1);
	}
	if (args[1][0] != '.' && args[1][0] != '/')
	{
		ft_putstr_fd("cd : only relative or absolute paths allowed\n", 2);
		shell_env->exit_status = 1;
		return (1);
	}
	if (chdir(args[1]) != 0)
	{
		ft_putstr_fd("cd : error changing directory\n", 2);
		shell_env->exit_status = 1;
		return (1);
	}
	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
	{
		if (oldpwd)
			env_set(shell_env, "OLDPWD", oldpwd);
		env_set(shell_env, "PWD", new_pwd);
	}
	shell_env->exit_status = 0;
	return (0);
}
