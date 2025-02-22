/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:43:00 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/04 15:43:00 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **argv, t_shell_env *shell_env)
{
	int	i;

	(void)argv;
	i = 0;
	while (shell_env->env[i])
	{
		printf("%s\n", shell_env->env[i]);
		i++;
	}
	return (0);
}
