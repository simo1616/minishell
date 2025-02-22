#include "minishell.h"

char	*search_in_path(char *cmd, char *path_env)
{
	char		**path_sp;
	char		*str;
	int			i;
	int			len;

	path_sp = ft_split(path_env, ':');
	if (!path_sp)
		return (NULL);
	i = 0;
	len = 0;
	while (path_sp[i]) // /home/simo1616/.local/bin:
	///usr/local/sbin = usr/local/sbin/ + cmd
	///usr/local/bin:/usr/sbin ...
	{
		len = ft_strlen(path_sp[i]);
		if (path_sp[i][len - 1] == '/') // verifier si contient déja un / a la fin 
			str = ft_strjoin(path_sp[i], cmd);
		else
			str = ft_strjoin_three(path_sp[i], "/", cmd);
		if (access(str, X_OK) == 0)
		{
			free_av(path_sp);
			return(str); // exemple on a trouver /usr/local/bin/ls
		}
		free(str);
		i++;
	}
	free_av(path_sp);
	return (NULL);
}
// recupere le contenu de PATH
char	*resolve_path(char *cmd, char **env)
{
	char	*path_env;
	int		i;

	if (!env)
		return (NULL);
	if (ft_strchr(cmd, '/')) // pou chemin directe entré par l'user
	{
		if (access(cmd, X_OK) == 0)
			return(ft_strdup(cmd));
		return (NULL);
	}
	path_env = NULL;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			/* la on a recup 
			/home/simo1616/.local/bin:/usr/local/sbin:
			/usr/local/bin:/usr/sbin:/usr/bin:/sbin:
			/bin:/usr/games:/usr/local/games:
			/snap/bin:/home/simo1616/.vscode/extensions/ms-python.debugpy
			-2025.0.0-linux-x64/bundled/scripts/noConfigScripts
			*/
			path_env = env[i] + 5;
			break;
		}
		i++;
	}
	if (!path_env) // si jamais il ne trouves pas le PATH dans la copie d'env
		return (NULL);
	return (search_in_path(cmd, path_env));
}

int excec_external(t_cmd *cmd, t_shell_env *shell_env)
{
	pid_t	pid;
    char	*path;
	int		status;

	path = resolve_path(cmd->av[0], shell_env->env); 
	if (!path)
	{
		ft_putstr_fd("Minishell: ", 2);
		write(2, cmd->av[0], ft_strlen(cmd->av[0]));
		ft_putstr_fd(": Aucun fichier ou dossier de ce nom\n", 2); // \n se debarasser peut etre trouver une autre facons d'erire le message d'erreur et la cmd 
		return (EX_CMD_NT_FD); 
	}
	pid = fork();
	if (pid < 0)
	{
		perror("minishell");
		shell_env->exit_status = 1;
		free(path);
		return (shell_env->exit_status);
	}
	if (pid == 0)
	{
		// POCESS CHILD
		execve(path, cmd->av, shell_env->env);
		perror("minishell : error Child process\n");
		free(path);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if(WIFEXITED(status)) //teste si le processus enfant s'est terminé normalement
		shell_env->exit_status = WEXITSTATUS(status); // extrait le code de sortie effectif du processus enfant. C'est ce nombre (souvent compris entre 0 et 255) 
	else if (WIFSIGNALED(status)) // teste si le processus enfant a été terminé par un signal
		shell_env->exit_status = EXIT_SIG_OFFSET + WTERMSIG(status); // 128 = commande terminer + WTERMSIG renvoie le numéro du signal ayant terminé le processus.
	free(path);
	return (shell_env->exit_status);
}

