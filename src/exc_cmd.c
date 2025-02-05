#include "minishell.h"

void execute_commands(t_cmd *cmds, t_shell_env *env)
{
    t_cmd 	*current;
	//t_redir *redir;
	int		ret;
	int 	i;


	current = cmds;
	i = 0;
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
				i = 0;
				printf("Commande: ");
				while(current->av[i])
				{
					printf("[%s] ", current->av[i]);
					i++;
				}
				printf("\n");
			}
		}
        // redir = current->redirs;
        // while (redir)
        // {
        //     printf("\nRedirection: type=%d, file=%s\n", 
        //            redir->type, redir->filename);
        //     redir = redir->next;
        // }
        current = current->next;
    }
}
// ancien excecute cmd pour debug :
// void execute_commands(t_cmd *cmds, t_shell_env *env)
// {
//     t_cmd 	*current;
// 	t_redir *redir;
// 	int 	i;


// 	current = cmds;
// 	(void)env;
// 	i = 0;
//     while (current)
//     {
//         printf("Command: ");
//         while(current->av[i])
// 		{
//             printf("[%s] ", current->av[i]);
// 			i++;
// 		}
//         redir = current->redirs;
//         while (redir)
//         {
//             printf("\nRedirection: type=%d, file=%s", 
//                    redir->type, redir->filename);
//             redir = redir->next;
//         }
//         printf("\n");
//         current = current->next;
//     }
// }