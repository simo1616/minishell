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
		path = env_get("HOME");
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


