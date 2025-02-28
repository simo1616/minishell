/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 19:27:04 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/28 17:20:49 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*remove_quotes(char *token) // pas utilisée
{
	int		len;
	char	*new_token;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len < 2)
		return (token);
	if ((token[0] == '\'' && token[len - 1] == '\'') || (token[0] == '"'
			&& token[len - 1] == '"'))
	{
		new_token = ft_substr(token, 1, len - 2);
		free(token);
		return (new_token);
	}
	return (token);
}

int	count_tokens(char *str)
{
	int		count;
	int		in_word;
	int		i;
	char	quote;

	count = 0;
	in_word = 0;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
		{
			if (!in_word)
			{
				in_word = 1;
				count++;
				if (str[i] == '\'' || str[i] == '"')
				{
					quote = str[i];
					i++;
					while (str[i] && str[i] != quote)
						i++;
					if (str[i] == quote)
						i++;
					in_word = 0;
				}
			}
			else
				i++;
		}
		else
		{
			in_word = 0;
			i++;
		}
	}
	return (count);
}