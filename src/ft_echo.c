/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:41:26 by jdecarro          #+#    #+#             */
/*   Updated: 2025/01/31 09:41:26 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option_echo(char *str)
{
	int i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

static void	write_echo(int i, bool new_line, char **args)
{
	bool	first = true;

	while (args[i])
	{
		if (!check_option_echo(args[i]))
		{
			if (!first)
				write(1, " ", 1);
			write(1, args[i], strlen(args[i]));
			first = false;
		}
		else
			new_line = false;
		i++;
	}
	if (new_line)
		write(1, "\n", 1);
}

int	ft_echo(char **args)
{
	int		i;
	bool	new_line;

	i = 1;
	new_line = true;
	while (args[i] && check_option_echo(args[i]))
	{
		new_line = false;
		i++;
	}
	write_echo(i, new_line, args);
	return (0);
}

int main(int argc, char **argv)
{
	ft_echo(argv);
	return (0);
}
