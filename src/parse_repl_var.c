/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_repl_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:05:27 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/13 18:05:32 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_exit_status(t_var *v)
{
	char	*exit_str;

	exit_str = ft_itoa(v->env->exit_status);
	ft_memcpy(v->new + v->j, exit_str, ft_strlen(exit_str));
	v->j += ft_strlen(exit_str);
	v->i++;
	v->data->cpos++;
	free(exit_str);
}

static void	handle_valid_var(t_var *v)
{
	char	*var_name;
	char	*var_value;
	int		var_start;

	var_start = v->i;
	while (v->token[v->i] && is_valid_var_char(v->token[v->i]))
	{
		v->i++;
		v->data->cpos++;
	}
	var_name = ft_substr(v->token, var_start, v->i - var_start);
	var_value = env_get(v->env, var_name);
	if (var_value)
	{
		ft_memcpy(v->new + v->j, var_value, ft_strlen(var_value));
		v->j += ft_strlen(var_value);
	}
	free(var_name);
}

static void	handle_dollar(t_var *v)
{
	v->i++;
	v->data->cpos++;
	if (!v->token[v->i])
		v->new[v->j++] = '$';
	else if (v->token[v->i] == '?')
		handle_exit_status(v);
	else if (!is_valid_var_char(v->token[v->i]))
	{
		v->new[v->j++] = '$';
		v->new[v->j++] = v->token[v->i++];
		v->data->cpos++;
	}
	else
		handle_valid_var(v);
}

static void	process_quoted_dollar(t_var *v)
{
	while (v->token[v->i] && v->data->ctx[v->data->cpos] == 1)
	{
		v->new[v->j++] = v->token[v->i++];
		v->data->cpos++;
	}
}

static void	process_characters(t_var *v)
{
	while (v->token[v->i])
	{
		if (v->token[v->i] == '$' && v->data->ctx[v->data->cpos] != 1)
			handle_dollar(v);
		else if (v->token[v->i] == '$')
			process_quoted_dollar(v);
		else
		{
			v->new[v->j++] = v->token[v->i++];
			v->data->cpos++;
		}
	}
}

char	*remplacer_var(char *token, t_shell_env *env, t_data *data)
{
	t_var	v;

	if (ft_strcmp(token, "$") == 0)
		return (ft_strdup(token));
	v.token = token;
	v.env = env;
	v.data = data;
	v.new = malloc(calculate_length(token, env, data->ctx) + 1);
	v.i = 0;
	v.j = 0;
	v.len = calculate_length(token, env, data->ctx);
	if (!v.new || !data->ctx)
	{
		if (v.new)
			free(v.new);
		return (NULL);
	}
	process_characters(&v);
	v.new[v.j] = '\0';
	return (v.new);
}
