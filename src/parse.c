#include "minishell.h"

char	*remplacer_var(char *token, t_shell_env *env)
{
	char	*new;
	char	*var_name;
	char	*var_value;

	if (ft_strcmp(token, "$?") == 0)
		return(ft_itoa(env->exit_status)); //rmeplacer le token par exitstaus
	if (ft_strcmp(token, "$") == 0)
		return(token);
	if (token[0] == '$') // si non variable $VAR ou $vide
	{
		var_name = token + 1;
		var_value = env_get(env, var_name);
		if (var_value)
			new = ft_strdup(var_value);
		else
			new = ft_strdup("");
		free(token);
		return(new);
	}
	return(token);
}

int	count_tokens(char *str)
{
	int count;
	int in_word;
	int i;

	count = 0;
	in_word = 0;
	i = 0;
	while(str[i])
	{
		if (!ft_isspace(str[i]) && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (ft_isspace(str[i]))
			in_word = 0;
		i++;
	}
	return(count);
}

char    *get_next_token(char **str, t_shell_env *env)
{
    char    *start;
    char    *token;
    int     len;
    int     i;

    len = 0;
    i = 0;
    while (ft_isspace((*str)[i]))
        i++;
    start = *str + i;
    while ((*str)[i] && !ft_isspace((*str)[i]))
    {
        len++;
        i++;
    }
    *str += i;
    token = ft_substr(start, 0, len);
	if(ft_strchr(token, '$'))			// pour echo $? ou autres $?
		token = remplacer_var(token, env); // remplacer $? par exit_status
    return (token);
}
// line = $? ls -l | grep lol | wc -l 
t_cmd	*parse_command_line(char *line, t_shell_env *env)
{
	t_cmd	*cmd;
	int		token_count;
	int		i;
	char	*current;

	if (!line)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if(!cmd)
		return(NULL);
	token_count = count_tokens(line);
	cmd->av = malloc(sizeof(char *) * (token_count + 1));
	if (!cmd->av)
    {
        free(cmd);
        return (NULL);
    }
	current = line;
	i = 0;
	while(i < token_count)
	{
		cmd->av[i] = get_next_token(&current, env);
		i++;
	}
	cmd->av[i] = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
	if (cmd->av[0] && cmd->av[0][0] == '\0')
    {
        printf("minishell: command not found: %s\n", cmd->av[0]);
        free(cmd->av);
        free(cmd);
        return (NULL);
    }
    
    return (cmd);
}
