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

// Une seule variable globale pour les signaux
extern int g_received_signal;

// Structure pour l'environnement du shell
typedef struct s_shell_env
{
    char    **env;          // Copie de l'environnement
    int     exit_status;    // Status de sortie de la dernière commande
    int     running;        // Flag pour contrôler l'exécution du shell
} t_shell_env;

// Prototypes
t_shell_env  *create_shell_env(char **envp);
void         destroy_shell_env(t_shell_env *shell_env);
void         signal_setup(void);

#endif