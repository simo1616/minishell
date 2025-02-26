#include "minishell.h"

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
	char	*exit_str;
	size_t	var_v_len;

	if (ft_strcmp(token, "$") == 0)
	{
		new = ft_strdup(token);
		return (new);
	}
	len_token = calculate_length(token, env, data->ctx);
	new = (char *)malloc(sizeof(char) * (len_token + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	var_len = 0;
	var_start = 0;
	var_v_len = 0;
	exit_str = NULL;
	if (!data->ctx)
		return (NULL);
	while (token[i])
	{
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
			if (!token[i])
			{
				new[j] = '$';
				j++;
				break ;
			}
			else if (token[i] == '?')
			{
				exit_str = ft_itoa(env->exit_status);
				ft_memcpy(new + j, exit_str, ft_strlen(exit_str));
				i++;
				data->cpos++;
				j += ft_strlen(exit_str);
				free(exit_str);
			}
			else if (!is_valid_var_char(token[i]))
			{
				new[j++] = '$';
				new[j++] = token[i];
				i++;
				data->cpos++;
			}
			else
			{
				var_start = i;
				while (token[i] && is_valid_var_char(token[i]))
				{
					i++;
					data->cpos++;
				}
				var_len = i - var_start;
				var_name = ft_substr(token, var_start, var_len);
				var_value = env_get(env, var_name);
				if (var_value)
				{
					var_v_len = ft_strlen(var_value);
					if (j + var_v_len > len_token)
					{
						ft_putstr_fd("Erreur: espace insuffisant dans le buffer pour copier \"", 2);
						ft_putstr_fd(var_value, 2);
						ft_putendl_fd("\"", 2);
						free(var_name);
						return (NULL);
					}
					ft_memcpy(new + j, var_value, var_v_len);
					j += var_v_len;
				}
				free(var_name);
			}
		}
		else if (token[i] == '$' && data->ctx[data->cpos] == 1)
		{
			while (token[i] && data->ctx[data->cpos] == 1)
			{
				new[j] = token[i];
				i++;
				j++;
			}
		}
	}
	new[j] = '\0';
	return (new);
}
