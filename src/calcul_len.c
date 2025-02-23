/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcul_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 09:52:34 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/23 10:26:24 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	process_exit_variable(t_shell_env *env)
{
	size_t	len;
	char	*exit_str;

	len = 0;
	exit_str = ft_itoa(env->exit_status);
	len = ft_strlen(exit_str);
	free(exit_str);
	return (len);
}

static size_t	process_normal_variable(char *token, t_shell_env *env, int *i)
{
	size_t	len;
	char	*var_name;
	char	*var_value;
	int		var_start;
	int		var_len;

	len = 0;
	var_start = *i;
	while (token[*i] && is_valid_var_char(token[*i]))
		(*i)++;
	var_len = *i - var_start;
	var_name = ft_substr(token, var_start, var_len);
	var_value = env_get(env, var_name);
	if (var_value)
		len = ft_strlen(var_value);
	free(var_name);
	return (len);
}

static size_t	process_variable(char *token, t_shell_env *env, int *i)
{
	size_t	len;

	len = 0;
	if (!token[*i])
		return (0);
	if (token[*i] == '?')
	{
		len = process_exit_variable(env);
		(*i)++;
	}
	else if (is_valid_var_char(token[*i]))
	{
		len = process_normal_variable(token, env, i);
	}
	else
	{
		len++;
		(*i)++;
	}
	return (len);
}

size_t	calculate_length(char *token, t_shell_env *env, int *ctx)
{
	int		i;
	size_t	len;

	if (!ctx)
		return ((size_t)(-1));
	i = 0;
	len = 0;
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
			i++;
		else if (token[i] == '$' && ctx[i] != 1)
		{
			i++;
			if (!token[i])
				break ;
			len += process_variable(token, env, &i);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
