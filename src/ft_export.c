/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 08:54:23 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/05 08:54:23 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(t_shell_env *shell_env, char *var)
{
	int		i;
	char	**new_env;

	i = 0;
	while (shell_env->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	i = 0;
	while (shell_env->env[i])
	{
		new_env[i] = strdup(shell_env->env[i]);
		free(shell_env->env[i]);
		i++;
	}
	new_env[i] = strdup(var);
	new_env[i + 1] = NULL;
	free(shell_env->env);
	shell_env->env = new_env;
}

void	print_env(t_shell_env *shell_env)
{
	int i;

	i = 0;
	while (shell_env->env[i])
	{
		printf("%s\n", shell_env->env[i]);
		i++;
	}
}

int	main(void)
{
	t_shell_env	shell_env;
	char		**init_env;

	init_env = malloc(sizeof(char *) * 3);
	init_env[0] = strdup("USER=student");
	init_env[1] = strdup("PATH=/usr/bin");
	init_env[2] = NULL;
	shell_env.env = init_env;

	printf("=== ENV AVANT EXPORT ===\n");
	print_env(&shell_env);

	// Ajout d'une nouvelle variable
	printf("\n=== EXPORT HOME ===\n");
	ft_export(&shell_env, "HOME=/home/user");
	print_env(&shell_env);

	// Modification de PATH
	printf("\n=== EXPORT PATH ===\n");
	ft_export(&shell_env, "PATH=/usr/local/bin");
	print_env(&shell_env);

	// Ajout d'une autre variable
	printf("\n=== EXPORT SHELL ===\n");
	ft_export(&shell_env, "SHELL=/bin/bash");
	print_env(&shell_env);

	// Libération mémoire
	int i = 0;
	while (shell_env.env[i])
		free(shell_env.env[i++]);
	free(shell_env.env);

	return (0);
}
