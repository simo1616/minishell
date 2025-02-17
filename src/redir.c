#include "minishell.h"

int	is_redir(char *token)
{
	return (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
	ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0);
}

t_redir_type	get_redir_type(char *token) //changer t_redir_type par int
{
	if (ft_strcmp(token, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(token, ">") == 0)
		return (REDIR_OUT);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIR_APPEND);
	if (ft_strcmp(token, "<<") == 0)
		return (REDIR_HEREDOC);
	return (REDIR_OUT);
}

void	add_redir_to_cmd(t_cmd *cmd, t_redir_type type, char *filename)
{
	t_redir *new_rd;
	t_redir *tmp;

	new_rd = malloc(sizeof(t_redir));
	if (!new_rd)
		return;
	new_rd->type = type;
	new_rd->filename = ft_strdup(filename);
	new_rd->next = NULL;
	if (cmd->redirs == NULL)
		cmd->redirs = new_rd;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_rd;
	}
	// Détermination de la chaîne de type selon le type passé en paramètre
	char *redir_type_str;
    if (type == REDIR_OUT)
        redir_type_str = "REDIR_OUT";
    else if (type == REDIR_IN)
        redir_type_str = "REDIR_IN";
    else if (type == REDIR_APPEND)
        redir_type_str = "REDIR_APPEND";
    else if (type == REDIR_HEREDOC)
        redir_type_str = "REDIR_HEREDOC";
    else
        redir_type_str = "INCONNU";

    printf("Redirection détectée : type = %s, filename = [%s]\n",
           redir_type_str, filename);
}

