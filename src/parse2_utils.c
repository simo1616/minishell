/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:01:13 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 19:16:12 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_quote(char c, int *quote)
{
	if (c == '\'' && *quote != 2)
	{
		if (*quote == 0)
			*quote = 1;
		else
			*quote = 0;
	}
	else if (c == '"' && *quote != 1)
	{
		if (*quote == 0)
			*quote = 2;
		else
			*quote = 0;
	}
}

static int	remplir_quotes_context(const char *line, int *context)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		check_quote(line[i], &quote);
		context[i++] = quote;
	}
	context[i] = 0;
	return (quote);
}

void	*get_quotes_context(t_data *data)
{
	if (!data->line)
		return (NULL);
	data->ctx = malloc(sizeof(int) * (ft_strlen(data->line) + 1));
	if (!data->ctx)
		return (NULL);
	if (remplir_quotes_context(data->line, data->ctx) != 0)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		free(data->ctx);
		return (NULL);
	}
	return (data);
}

static char	**create_new_argv(char **av, int len)
{
	char	**new_av;
	int		i;

	new_av = malloc(sizeof(char *) * (len + 2));
	if (!new_av)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_av[i] = av[i];
		i++;
	}
	return (new_av);
}

char	**add_to_argv(char **av, const char *token)
{
	char	**new_av;
	int		len;

	if (!av || !av[0])
		return (init_argv(token));
	len = 0;
	while (av[len])
		len++;
	new_av = create_new_argv(av, len);
	if (!new_av)
		return (NULL);
	new_av[len] = ft_strdup(token);
	new_av[len + 1] = NULL;
	free(av);
	return (new_av);
}
