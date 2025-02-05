#include "minishell.h"

int	is_builtin(char	*cmd_name)
{
	t_builtin 	*builtins;
	int			i;
	
	builtins = init_builtins();
	i = 0;
	while(builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, cmd_name) == 0)
			return (1);
		i++;
	}
	return(0);
}


int	excec_builin(t_cmd *cmd, t_shell_env *shell_env)
{
	t_builtin *	builtins;
	int			i;

	builtins = init_builtins();
	i = 0;
	while(builtins[i].name)
	{
		if(ft_strcmp(builtins[i].name, cmd->av[0]) == 0)
			return(builtins[i].func(cmd->av, shell_env));
		i++;
	}
	return(0);
}