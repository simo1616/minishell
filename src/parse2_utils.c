#include "minishell.h"

void	*get_quotes_context(t_data *data)
{
	int	i;
	int	*context;
	int	quote;

	if (!data->line)
		return (NULL);
	context = (int *)malloc(sizeof(int) * (ft_strlen(data->line) + 1));
	if (!context)
		return (NULL);
	quote = 0;
	i = 0;
	while (data->line[i])
	{
		if (data->line[i] == '\'' && quote != 2)
		{
			if (quote == 0)
				quote = 1;
			else
				quote = 0;
		}
		else if (data->line[i] == '"' && quote != 1)
		{
			if (quote == 0)
				quote = 2;
			else
				quote = 0;
		}
		context[i] = quote;
		i++;
	}
	context[i] = 0;
	data->ctx = context;
	if (quote != 0)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		free(data->ctx);
		return (NULL);
	}
	return (data);
}

char	**add_to_argv(char **av, const char *token)
{
	char	**new_av;
	int		len;
	int		i;

	len = 0;
	if (av)
	{
		while (av[len])
			len++;
	}
	if (len == 0 && av == NULL)
	{
		new_av = malloc(sizeof(char *) * 2);
		new_av[0] = ft_strdup(token);
		new_av[1] = NULL;
		return (new_av);
	}
	new_av = malloc(sizeof(char *) * (len + 2));
	if (!new_av)
		return (NULL);
	i = -1;
	while (++i < len)
		new_av[i] = av[i];
	new_av[len] = ft_strdup(token);
	new_av[len + 1] = NULL;
	if (av)
		free(av);
	return (new_av);
}