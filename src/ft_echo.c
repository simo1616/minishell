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

static bool	check_option_echo(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (!str[i])
		return (false);
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

static void	write_echo(int i, bool new_line, char **args)
{
	bool	first;

	first = true;
	while (args[i])
	{
		if (!first)
			write(1, " ", 1);
		write(1, args[i], ft_strlen(args[i]));
		first = false;
		i++;
	}
	if (new_line)
		write(1, "\n", 1);
}

int	ft_echo(char **args, t_shell_env *shell_env)
{
	int		i;
	bool	new_line;

	(void)shell_env;
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

// int main(int argc, char **argv)
// {
// 	ft_echo(argv);
// 	return (0);
// }
