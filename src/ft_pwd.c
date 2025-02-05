/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:24:10 by jdecarro          #+#    #+#             */
/*   Updated: 2025/01/31 14:24:10 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	ft_pwd(char **argv, t_shell_env *shell_env)
{
	char	cwd[PATH_MAX];

	(void)argv;
	(void)shell_env;
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

// int	main(int argc, char **argv)
// {
// 	ft_pwd();
// 	return (0);
// }
// int main ()
// {
// 	int i;
// 	i = 0;
// 	t_builtin g_builtins[] = {
// 		{"cd",     &builtin_cd},
// 		{"exit",   &builtin_exit},
// 		{"pwd",    &builtin_pwd},
// 		{"echo",   &builtin_echo},
// 		{"env",    &builtin_env},
// 		{"export", &builtin_export},
// 		{"unset",  &builtin_unset},
// 		{NULL,     NULL}
// 	};
// }
