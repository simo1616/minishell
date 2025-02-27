/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:34:06 by jdecarro          #+#    #+#             */
/*   Updated: 2025/02/27 13:55:11 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_unset_name(char *args)
{
	int	i;

	if (!args || !args[0] || args[0] == '=' || ft_isdigit(args[0]))
		return (0);
	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(char **args, t_shell_env *shell_env)
{
	int	i;
	int	j;
	int	len;

	i = 1;
	if (!args[1])
		return (0);
	if (!check_unset_name(args[1]))
	{
		ft_printf("minishell: unset: `%s': not a valid identifier\n", args[1]);
		return (1);
	}
	while (args[i])
	{
		len = ft_strlen(args[i]);
		j = 0;
		while (shell_env->env[j])
		{
			if (ft_strncmp(shell_env->env[j], args[i], len) == 0
				&& shell_env->env[j][len] == '=')
			{
				free(shell_env->env[j]);
				while (shell_env->env[j])
				{
					shell_env->env[j] = shell_env->env[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}
