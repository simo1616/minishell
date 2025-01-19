#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    char *cmd_line;
    t_shell_env *shell_env;

    (void)ac;
    (void)av;

    shell_env = create_shell_env(envp);
    if (!shell_env)
        return (1);
    signal_setup();

    while (shell_env->running)
    {
        cmd_line = readline("minishell> ");
        if (!cmd_line) // Gestion de Ctrl+D
        {
            printf("exit\n");
            break;
        }
        if (*cmd_line)
            add_history(cmd_line);

        // Gestion des signaux reçus
        if (g_received_signal)
        {
            if (g_received_signal == SIGINT)
                shell_env->exit_status = 130;
            g_received_signal = 0;
        }

        // TODO: Parsing et exécution à implémenter
		// ls -l -> args a/out | & ; cat -e 
		// execve(args, &env,null)
        
        free(cmd_line);
    }
    
    destroy_shell_env(shell_env);
    return (shell_env->exit_status);
}