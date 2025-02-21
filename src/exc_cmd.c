#include "minishell.h"

void execute_commands(t_cmd *cmds, t_shell_env *env)
{
    t_cmd 	*current;
	t_redir *redir;
	int		ret;
	//int 	i; // debug


	current = cmds;
    while (current)
    {
        if(current->av && current->av[0])
		{
			//printf("is_builtin(%s) = %d\n", current->av[0], is_builtin(current->av[0]));
			if(is_builtin(current->av[0]))
			{
				ret = excec_builin(current, env);
				env->exit_status = ret;
			}
			else
			{
				ret = excec_external(current, env);
				env->exit_status = ret; 
				// si c'est pas external 
				// i = 0; //debug
				// printf("Commande: "); //debug
				// while(current->av[i])
				// {
				// 	printf("[%s] ", current->av[i]);
				// 	i++;
				// }
				// printf("\n");
			}
		}
        redir = current->redirs;
        while (redir)
        {
            printf("\nRedirection: type=%d, file=%s\n", 
                   redir->type, redir->filename);
            redir = redir->next;
        }
        current = current->next;
    }
}
