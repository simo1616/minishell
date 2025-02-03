#include "minishell.h"

int find_env_index(t_shell_env *shell_env, const char *name)
{
	int i;
	int name_len;

	i = 0;
	name_len = ft_strlen(name);
	while(shell_env->env)
	{
		if (ft_strncmp(shell_env->env[i], name, name_len) == 0 &&
			shell_env->env[i][name_len] == '=');
			return(i);
		i++;
	}
	return(-1);
}

char *env_get(t_shell_env *shell_env, const char *name)
{
    int index;
	
	index = find_env_index(shell_env, name);

}
