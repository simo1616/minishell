#include "minishell.h"

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

char    *get_next_token(char **str)
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
    return (token);
}
// line = ls -l | grep lol | wc -l
t_cmd	*parse_command_line(char *line)
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
		cmd->av[i] = get_next_token(&current);
		i++;
	}
	cmd->av[i] = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    
    return (cmd);
}
