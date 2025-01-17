#include "minishell.h"

char *ft_strtok(char *str, const char *delim)
{
    static char *last_str;
    int i;
    int j;
    char *token_start;
    
    if (str)
        last_str = str;
    if (!last_str)
        return NULL;
    i = 0;
    while (last_str[i])
    {
        j = 0;
        while (delim[j])
        {
            if (last_str[i] == delim[j])
                break;
            j++;
        }
        if (!delim[j])
            break;
        i++;
    }
    
    if (last_str[i] == '\0')
    {
        last_str = NULL;
        return NULL;
    }
    token_start = &last_str[i];
    while (last_str[i])
    {
        j = 0;
        while (delim[j])
        {
            if (last_str[i] == delim[j])
                break;
            j++;
        }
        if (delim[j])
            break;
        i++;
    }
    
    if (last_str[i] == '\0')
        last_str = NULL;
    else
    {
        last_str[i] = '\0';
        last_str = &last_str[i + 1];
    }
    
    return token_start;
}