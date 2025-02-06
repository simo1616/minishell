#include "minishell.h"


char	*remove_quotes(char *token)
{
	int		len;
	char	*new_token;

	if(!token)
		return(NULL);
	len = ft_strlen(token);
	if(len < 2)
		return(token);
	if((token[0] == '\'' && token[len - 1] == '\'') ||
		(token[0] == '"' && token[len - 1] == '"'))
	{
		new_token = ft_substr(token, 1, len - 2);
		free(token);
		return(new_token);
	}
	return(token);
}

int	check_quotes(char *str)
{
	int		i;
	int 	status_s;
	int		status_d;

	if(!str)
		return(0);
	i = 0;
	status_s = 1;
	status_d = 1;
	if (str[0] == '"' || str[0] == '\'')
	{
		printf("minishell: command not found: %s\n", str);
		return (0);
	}
	while(str[i])
	{ // ' "test" "" ' // "echo 'hello"

		if (str[i] == '\'' && status_d == 1)
			status_s = !status_s;
		else if (str[i] == '"' && status_s == 1)
			status_d = !status_d;
		i++;
	}
	if(status_s == 0 || status_d == 0)
	{
		printf("minishell: syntax error: unclosed quote\n");
		return(0);
	}
	return(1);
}


char	*remplacer_var(char *token, t_shell_env *env)
{
	char	*new;
	char	*var_name;
	char	*var_value;

	if (ft_strcmp(token, "$?") == 0)
		return(ft_itoa(env->exit_status)); //rmeplacer le token par exitstaus
	if (ft_strcmp(token, "$") == 0)
		return(token);
	if (token[0] == '$') // si non variable $NON_VAR ou $vide
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
	int		count;
	int		in_word;
	int		i;
	char	quote;

	count = 0;
	in_word = 0;
	i = 0;
	quote = 0;
	while(str[i])
	{
		if(!ft_isspace(str[i]))
		{
			if (!in_word)
			{
				in_word = 1;
				count++;
				if(str[i] == '\'' || str[i] == '"')
				{
					quote = str[i];
					i++;
					while(str[i] && str[i] != quote)
						i++;
					if(str[i] == quote)
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
	//printf("count token%d\n", count);
	return(count);
}

char *get_next_token(char **str, t_shell_env *env) 
{
    char *start;
    char *token;
    int len = 0;
    int i = 0;
    char quote = 0;

    while (ft_isspace((*str)[i]))
        i++;
    start = *str + i;
    while ((*str)[i]) 
	{
        if ((*str)[i] == '\'' || (*str)[i] == '"') 
		{
            if (!quote)
                quote = (*str)[i];
            else if (quote == (*str)[i])
                quote = 0;
        }
        if (!quote && ft_isspace((*str)[i]))
            break;
        len++;
        i++;
    }
    *str += i;
    token = ft_substr(start, 0, len);
    if (ft_strchr(token, '$'))
        token = remplacer_var(token, env);
    token = remove_quotes(token);
    return token;
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
	if (!check_quotes(current))
	{
        free(cmd);
        return (NULL);
    }
	i = 0;
	while(i < token_count)
	{
		cmd->av[i] = get_next_token(&current, env);
		i++;
	}
	cmd->av[i] = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    
    return (cmd);
}
