#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>

// Une seule variable globale pour les signaux
extern int g_received_signal;

// Structure pour l'environnement du shell
typedef struct s_shell_env
{
    char    **env;          // Copie de l'environnement
    int     exit_status;    // Status de sortie de la dernière commande
    int     running;        // Flag pour contrôler l'exécution du shell
} t_shell_env;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct	s_redir	*next;
}	t_redir;

typedef struct s_cmd 
{
	char			**av;
	t_redir			*redirs;
	struct s_cmd	*next;
} t_cmd;

t_shell_env	*create_shell_env(char **envp);
void		destroy_shell_env(t_shell_env *shell_env);
void		signal_setup(void);
t_cmd 		*parse_command_line(char *line);
void		execute_commands(t_cmd *cmds, t_shell_env *shell_env);
int			count_tokens(char *str);
char		*get_next_token(char **str);
void 		free_cmds(t_cmd *cmds);
void 		free_av(char **argv);
void 		free_redirs(t_redir *redirs);

// fonction env
int 		find_env_index(t_shell_env *shell_env, const char *name);
char 		*env_get(t_shell_env *shell_env, const char *name);
char		*ft_strjoin_three(const char *name, const char *eq, const char *value);
int 		env_set(t_shell_env *shell_env, const char *name, const char *value);
int 		env_unset(t_shell_env *shell_env, const char *name);

//builting
int			ft_echo(char **args);
int			ft_pwd(void);
//int			ft_env(t_env *env);

#endif