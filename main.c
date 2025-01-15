#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
//#include "./libft/inc/libft.h"
#include "./libft/inc/get_next_line.h"

#define MAX_CMD_LEN 1024

char **parse_cmd(char *cmd)
{
    char **args = malloc(sizeof(char *) * 64);
    int i = 0;
    
    char *token = strtok(cmd, " \n");
    while (token != NULL && i < 63)
    {
        args[i] = strdup(token);
        token = strtok(NULL, " \n");
        i++;
    }
    args[i] = NULL;
    return args;
}

void    free_args(char **args)
{
    int i;
    
    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

int main(void)
{
    char    *cmd;
    char    **args;
    pid_t   pid;
    
    while (1)
    {
        write(1, "minishell> ", 11);
        cmd = get_next_line(0);
        if (!cmd)
            break;
        if (cmd[0] == '\n')
        {
            free(cmd);
            continue;
        }
        
        args = parse_cmd(cmd);
        free(cmd);
        if (!args[0])
        {
            free_args(args);
            continue;
        }
        
        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            free_args(args);
            continue;
        }
        
        if (pid == 0)
        {
            execvp(args[0], args);
            perror("exec failed");
            free_args(args);
            exit(1);
        }
        else
            wait(NULL);
        
        free_args(args);
    }
    write(1, "\nExit minishell\n", 15);
    return (0);
}