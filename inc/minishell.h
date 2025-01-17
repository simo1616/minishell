#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>

# define MAX_CMD_LEN 1024

char    *ft_strtok(char *str, const char *delim);
char    **parse_cmd(char *cmd);
void    free_args(char **args);

#endif