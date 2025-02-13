#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
/* 	calcule la taille qu'on dois allouer pour le token entre des " ou ' 
	si return NULL
"*/
size_t	calculate_length(const char *token, t_shell_env *env, int *ctx)
{
	int		i;
	size_t	len;
	int		var_start;
	int		var_len;
	char	*var_name;
	char	*var_value;
	char 	*exit_str;

	if(!ctx)
		return((size_t)(-1));
	i = 0;
	len = 0;
	var_start = 0;
	var_len = 0;
	var_name = NULL;
	var_value = NULL;
	exit_str = NULL;
	while(token[i])
	{
		if(token[i] == '\'' || token[i] == '"')
			i++;
		else if (token[i] == '$' && ctx[i] != 1)
		{
			i++;
			if (!token[i])
				break;
			else if (token[i] == '?')
			{
				exit_str = ft_itoa(env->exit_status);
				len += ft_strlen(exit_str);
				free(exit_str);
				i++;
			}
			else if (token[i] && is_valid_var_char(token[i]))
			{
				var_start = i;
				while (token[i] && is_valid_var_char(token[i]))
					i++;
				var_len = i - var_start;
				var_name = ft_substr(token, var_start, var_len);
				var_value = env_get(env, var_name);
				if(var_value)
					len += ft_strlen(var_value);
				free(var_name); 
			}
			else
				len++;
		}
		else
		{
			len++;
			i++;
		}
	}
	//free(ctx);
	return(len);
}

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
// 0: aucun, 1: simple, 2: double
void	*get_quotes_context(t_data *data)
{
	int		i;
	int		*context;
	int 	quote; 

	if(!data->line)
		return(NULL);
	context = (int *)malloc(sizeof(int) * (ft_strlen(data->line) + 1));
	if(!context)
		return(NULL);
	quote = 0;
	i = 0;
	while(data->line[i])
	{ // ' "test" "" ' // "echo 'hello"

		if (data->line[i] == '\'' && quote != 2) // on est dans une simple et pas double
		{
			if(quote == 0)
				quote = 1;
			else
			quote = 0;
		}
		else if (data->line[i] == '"' && quote != 1)
		{
			if(quote == 0)
				quote = 2;
			else
				quote = 0;
		}
		context[i] = quote;
		i++;
	}
	context[i] = 0; // pour parcourire la chaine aprés
	data->ctx = context;
	i = 0;
	while((size_t)i < (ft_strlen(data->line) + 1))
	{
		printf("data->ctx[%d] = %d\n", i, data->ctx[i]);
		i++;
	}
	if(quote != 0)
	{
		printf("minishell: syntax error: unclosed quote\n");
		free(data->ctx);
		return(NULL);
	}
	printf("data.line dans GQC= %s\n", data->line); // debug
	return(data);
}

char	*remplacer_var(char *token, t_shell_env *env, t_data *data)
{
	char	*new;
	char	*var_name;
	char	*var_value;
	int		var_start;
	int		var_len;
	size_t	len_token;
	int		i;
	int		j;
	char 	*exit_str;

	if (ft_strcmp(token, "$") == 0)
		return(token);
	len_token = calculate_length(token, env, data->ctx);
	printf("=====> calculate_length(token, env) = %ld\n", len_token);
	new = (char *)malloc(sizeof(char) * (len_token + 1));
	if(!new)
		return NULL;
	i = 0;
	j = 0;
	var_len = 0;
	var_start = 0;
	exit_str = NULL;
	if(!data->ctx)
		return (NULL);
	while(token[i])
	{
		printf("token[%d] = %c et ctx[%d] = %d\n", i, token[i], i, data->ctx[data->cpos]);
		if (token[i] != '$')
		{
			new[j] = token[i];
			i++;
			j++;
			data->cpos++;
		}
		else if (token[i] == '$' && data->ctx[data->cpos] != 1)
		{
			i++;
			data->cpos++;
			if(token[i] == '?')
			{
				exit_str = ft_itoa(env->exit_status);
				ft_memcpy(new + j, exit_str, ft_strlen(exit_str));
				i++;
				data->cpos++;
				j += ft_strlen(exit_str);
				free(exit_str);
			}
			else
			{
				var_start = i;
				while(token[i] && is_valid_var_char(token[i]))
				{
					i++;
					data->cpos++;
				}
				var_len = i - var_start;
				var_name = ft_substr(token, var_start, var_len);
				var_value = env_get(env, var_name);
				if (var_value)
				{
					ft_memcpy(new + j, var_value, ft_strlen(var_value)); // ici
					j += ft_strlen(var_value);
				}
				free(var_name);
			}
		}
		else if (token[i] == '$' && data->ctx[data->cpos] == 1)
		{
			while(token[i] && data->ctx[data->cpos] == 1)
			{
				new[j] = token[i];
				i++;
				data->cpos++;
				j++;
			}
		}
		
	}
	new[j] = '\0';
	//free(ctx);
	return(new);
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

char *get_next_token(char **str, t_shell_env *env, t_data *data) 
{
	char	*token;
	char	*buffer;
    int		pos;
    int		len;
    int		quote; // 0: aucun, 1: simple, 2: double
	int		total;

    pos = 0;
	printf("str = %s\n", *str); // debug
    while ((*str)[pos] && ft_isspace((*str)[pos]))
        pos++;
    *str += pos;
	pos = 0;
	len = 0;
	quote = 0;
	total = ft_strlen(*str);
	if (!total)
        return NULL;
	buffer = (char *)malloc(sizeof(char) * (total + 1));
	if (!buffer)
        return NULL;
    while (pos < total) 
	{
        if ((*str)[pos] == '\'' || (*str)[pos] == '"') 
		{
            if (!quote)
			{
				if ((*str)[pos]== '\'')
					quote = 1;
				else if ((*str)[pos] == '\"')
					quote = 2;
			}
            else if ((quote == 1 && (*str)[pos] == '\'') ||
				(quote == 2 && (*str)[pos] == '"'))
                quote = 0;
			else
			{
				buffer[len] = (*str)[pos];
				len++;
			}
			pos++;
        }
		else
		{
			if (!quote && ft_isspace((*str)[pos]))
				break;
			buffer[len] = (*str)[pos];
			len++;
			pos++;
		}
    }
	buffer[len] = '\0';
    *str += pos;
	if (ft_strchr(buffer, '$'))
    {
        token = remplacer_var(buffer, env, data);
        free(buffer);
    }
    else
	{

        token = buffer;
	}
	
	printf("token = %s\n", token); // debug
    return (token);
}

// line = $? ls -l | grep lol | wc -l 
t_cmd	*parse_command_line(char *line, t_shell_env *env)
{
	t_cmd	*cmd;
	t_data	data;
	int		token_count;
	int		i;
	//char	*current;

	if (!line)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if(!cmd)
		return(NULL);
	token_count = count_tokens(line);
	cmd->av = malloc(sizeof(char *) * (token_count + 1));
	if (!cmd->av)
    {
		free(data.ctx);
        free(cmd);
        return (NULL);
    }
	data.line = line;
	data.cpos = 0;
	printf("data.line = %s\n", data.line); // debug
	if (!get_quotes_context(&data))
	{
        free(cmd);
        return (NULL);
    }
	i = 0;
	while(i < token_count)
	{
		cmd->av[i] = get_next_token(&data.line, env, &data);
		i++;
	}
	cmd->av[i] = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    
    return (cmd);
}
