#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

size_t	calculate_length(const char *token, t_shell_env *env, int *ctx)
{
	int		i;
	size_t	len;
	int		var_start;
	int		var_len;
	char	*var_name;
	char	*var_value;
	char	*exit_str;

	if (!ctx)
		return ((size_t)(-1));
	i = 0;
	len = 0;
	var_start = 0;
	var_len = 0;
	var_name = NULL;
	var_value = NULL;
	exit_str = NULL;
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
			i++;
		else if (token[i] == '$' && ctx[i] != 1)
		{
			i++;
			if (!token[i])
				break ;
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
				if (var_value)
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
	return (len);
}

char	*remove_quotes(char *token)
{
	int		len;
	char	*new_token;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len < 2)
		return (token);
	if ((token[0] == '\'' && token[len - 1] == '\'') || (token[0] == '"'
			&& token[len - 1] == '"'))
	{
		new_token = ft_substr(token, 1, len - 2);
		free(token);
		return (new_token);
	}
	return (token);
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
	while (str[i])
	{
		if (!ft_isspace(str[i]))
		{
			if (!in_word)
			{
				in_word = 1;
				count++;
				if (str[i] == '\'' || str[i] == '"')
				{
					quote = str[i];
					i++;
					while (str[i] && str[i] != quote)
						i++;
					if (str[i] == quote)
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
	return (count);
}
