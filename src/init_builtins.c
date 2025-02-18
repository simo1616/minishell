#include "minishell.h"

static t_builtin *builtins = NULL;

t_builtin 	*init_builtins(void)
{
	if (builtins == NULL)
	{
		builtins = malloc(sizeof(t_builtin) * 8);
		if(!builtins)
		{
			perror("error malloc builtins");
			exit(EXIT_FAILURE);
		}
		builtins[0].name = ft_strdup("cd");
		builtins[0].func = &ft_cd;
		builtins[1].name = ft_strdup("exit");
		builtins[1].func = &ft_exit;
		builtins[2].name = ft_strdup("pwd");
		builtins[2].func = &ft_pwd;
		builtins[3].name = ft_strdup("echo");
		builtins[3].func = &ft_echo;
		builtins[4].name = ft_strdup("env");
		builtins[4].func = &ft_env;
		builtins[5].name = ft_strdup("export");
		builtins[5].func = &ft_export;
		builtins[6].name = ft_strdup("unset");
		builtins[6].func = &ft_unset;
		builtins[7].name = NULL;
		builtins[7].func = NULL;
	}
	return(builtins);
}
