/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:28:15 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/14 15:24:26 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	**init_argv(const char *token)
{
	char	**new_av;

	new_av = malloc(sizeof(char *) * 2);
	if (!new_av)
		return (NULL);
	new_av[0] = ft_strdup(token);
	new_av[1] = NULL;
	return (new_av);
}

void	handle_exit_status(t_var *v)
{
	char	*exit_str;

	exit_str = ft_itoa(v->env->exit_status);
	ft_memcpy(v->new + v->j, exit_str, ft_strlen(exit_str));
	v->j += ft_strlen(exit_str);
	v->i++;
	v->data->cpos++;
	free(exit_str);
}
