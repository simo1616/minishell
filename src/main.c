#include "minishell.h"

int main (int ac, char **av, char **envp)
{
	char		*cmd_line;
	t_shell_env	*shell_env;
	t_cmd		*cmds;
	int 		ret;

	(void)ac;
	(void)av;

	shell_env = create_shell_env(envp);
	if (!shell_env)
		return (1);
	signal_setup();

	while (shell_env->running)
	{
		cmd_line = readline("\001\033[1;35m\002minishell> \001\033[0m\002");
		//cmd_line = readline("\033[38;5;004mminishell> \033[0m");
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
		// parsing
		cmds = parse_command_line(cmd_line, shell_env);
		if (cmds == NULL)
		{
			// Gestion d'erreur déjà présente
			shell_env->exit_status = 258; // Code d'erreur syntaxique
		}
		else if (cmds->next)
		{
			excec_pipes(cmds, shell_env);
		}
		else
		{
			execute_commands(cmds, shell_env);
			free_cmds(cmds);
		}
		
		// excecuting 
		// execute_commands(cmds, shell_env);
		// free_cmds(cmds);
		free(cmd_line);
	}
	ret = shell_env->exit_status;
	destroy_shell_env(shell_env);
	shell_env = NULL;
	return (ret);
}
