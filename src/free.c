// Libère une liste chaînée de redirections
#include "minishell.h"

void free_redirs(t_redir *redirs)
{
    t_redir *current;
    t_redir *next;

    current = redirs;
    while (current)
    {
        next = current->next;
        // On libère d'abord le nom du fichier qui a été alloué
        if (current->filename)
            free(current->filename);
        // Puis la structure de redirection elle-même
        free(current);
        current = next;
    }
}

// Libère le tableau d'arguments d'une commande
void free_av(char **argv)
{
    int i;

    if (!argv)
        return;
    // On parcourt le tableau jusqu'à NULL
    i = 0;
    while (argv[i])
    {
        free(argv[i]);  // Libération de chaque argument
        i++;
    }
    free(argv);        // Libération du tableau lui-même
}

// Fonction principale qui libère toute la liste de commandes
void free_cmds(t_cmd *cmds)
{
    t_cmd *current;
    t_cmd *next;

    current = cmds;
    while (current)
    {
        next = current->next;
        // On libère d'abord les redirections de la commande
        if (current->redirs)
            free_redirs(current->redirs);
        // Puis les arguments
        if (current->av)
            free_av(current->av);
        // Enfin la structure de commande elle-même
        free(current);
        current = next;
    }
}