/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:38:15 by mbendidi          #+#    #+#             */
/*   Updated: 2025/02/22 18:48:52 by mbendidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin *init_builtins(t_shell_env *shell_env)
{
    if (shell_env->builtins == NULL)
    {
        shell_env->builtins = malloc(sizeof(t_builtin) * 8);
        if (!shell_env->builtins)
        {
            perror("error malloc builtins");
            exit(EXIT_FAILURE);
        }
        shell_env->builtins[0].name = ft_strdup("cd");
        shell_env->builtins[0].func = &ft_cd;
        shell_env->builtins[1].name = ft_strdup("exit");
        shell_env->builtins[1].func = &ft_exit;
        shell_env->builtins[2].name = ft_strdup("pwd");
        shell_env->builtins[2].func = &ft_pwd;
        shell_env->builtins[3].name = ft_strdup("echo");
        shell_env->builtins[3].func = &ft_echo;
        shell_env->builtins[4].name = ft_strdup("env");
        shell_env->builtins[4].func = &ft_env;
        shell_env->builtins[5].name = ft_strdup("export");
        shell_env->builtins[5].func = &ft_export;
        shell_env->builtins[6].name = ft_strdup("unset");
        shell_env->builtins[6].func = &ft_unset;
        shell_env->builtins[7].name = NULL;
        shell_env->builtins[7].func = NULL;
    }
    return (shell_env->builtins);
}

