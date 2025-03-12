/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdecarro <jdecarro@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:05:11 by mbendidi          #+#    #+#             */
/*   Updated: 2025/03/12 11:15:41 by jdecarro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>

extern int					g_received_signal;

// macro EXIT
# define EX_CMD_NT_FD 127 // COMMANDE NOT FOUND
# define EX_CMD_NT_EXE 126 // COMMANDE NOT EXECUTABLE
# define EXIT_SIG_OFFSET 128 // processus s'est terminer à cause d'un signal

typedef struct s_builtin	t_builtin;

// Structure copie d'environnement du shell
typedef struct s_shell_env
{
	char					**env;
	int						exit_status;
	int						running;
	t_builtin				*builtins;
}							t_shell_env;

/*
	IN < OUT > APPEND, >> HEREDOC <<
*/
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
}							t_redir_type;

typedef struct s_redir
{
	t_redir_type			type;
	char					*filename;
	struct s_redir			*next;
}							t_redir;

typedef struct s_cmd
{
	char					**av;
	t_redir					*redirs;
	struct s_cmd			*next;
}							t_cmd;

struct						s_builtin
{
	char					*name;
	int						(*func)(char **av, t_shell_env *shell_env);
};

typedef struct s_data
{
	char					*line;
	int						*ctx;
	size_t					cpos;
}							t_data;

typedef struct s_tokenizer
{
	t_data					*data;
	t_shell_env				*env;
	char					*buffer;
	int						*token_ctx;
	int						pos;
	int						len;
	int						quote;
	int						total;
	int						*original_ctx;
	size_t					ctx_start;
}	t_tokenizer;

typedef struct s_vars
{
	t_shell_env				*env;
	t_data					*data;
	char					*token;
	char					*new_str;
	int						i;
	int						j;
}	t_vars;

typedef struct s_calc
{
	t_shell_env				*env;
	size_t					len;
	char					*token;
	int						*ctx;
	int						i;
}	t_calc;

typedef struct s_token_data
{
	char					*token;
	int						*token_ctx;
	size_t					len;
}	t_token_data;

typedef struct s_pipe_info
{
	int						prev_fd;
	int						fd[2];
	pid_t					*pids;
	int						cmd_count;
}	t_pipe_info;

// copie env
t_shell_env					*create_shell_env(char **envp);
void						destroy_shell_env(t_shell_env *shell_env);
void						signal_setup(void);

// parsing
t_cmd						*parse_command_line(char *line, t_shell_env *env);
int							handle_pipe(t_cmd **cur_cmd);
int							handle_redir(char *token, t_data *data,
								t_shell_env *env, t_cmd **cur_cmd);
int							process_token(char *token, t_data *data,
								t_shell_env *env, t_cmd **cur_cmd,
								int *token_ctx, size_t len);
t_cmd						*parse_tokens(t_data *data, t_shell_env *env);
int							init_cmd_ifneed(t_cmd **cmd, t_cmd **cur_cmd);
char						**init_argv(const char *token);
int							final_verification(t_cmd *cmd);
char						**add_to_argv(char **av, const char *token);
void						*get_quotes_context(t_data *data);
void						execute_commands(t_cmd *cmds,
								t_shell_env *shell_env);
int							count_tokens(char *str);
t_token_data				get_next_token(t_shell_env *env, t_data *data);
char						*expand_token(t_tokenizer *tok);
void						fill_buffer_and_ctx(t_tokenizer *tok);
int							alloc_buffer(t_tokenizer *tok);
char						*remplacer_var(char *token, t_shell_env *env,
								t_data *data);
char						*remove_quotes(char *token);

// expansion
size_t						calculate_length(char *token,
								t_shell_env *env, int *ctx);
int							is_valid_var_char(char c);

// redir
int							is_redir(char *token);
t_redir_type				get_redir_type(char *token);
void						add_redir_to_cmd(t_cmd *cmd, t_redir_type type,
								char *filename);
int							handle_redirections(t_cmd *cmd);

// pipes
int							exec_pipes(t_cmd *cmds, t_shell_env *env);
int							init_pipe_info(t_pipe_info *pipes, t_cmd *cmds);
void						child_process_pipes(t_cmd *cur,
								t_shell_env *env, t_pipe_info *pipes);
void						parent_process_pipes(t_pipe_info *pipes,
								t_cmd *cur);
void						wait_for_children(pid_t *pids, int count,
								int *status, t_shell_env *env);

// free
void						free_cmds(t_cmd *cmds);
void						free_av(char **argv);
void						free_redirs(t_redir *redirs);
void						free_builtins(t_builtin *builtins);

// fonction env
int							find_env_index(t_shell_env *shell_env,
								const char *name);
char						*env_get(t_shell_env *shell_env, const char *name);
char						*ft_strjoin_three(const char *name, const char *eq,
								const char *value);
int							env_set(t_shell_env *shell_env, const char *name,
								const char *value);
int							env_unset(t_shell_env *shell_env, const char *name);

// builting
int							ft_echo(char **args, t_shell_env *shell_env);
int							ft_pwd(char **argv, t_shell_env *shell_env);
int							ft_env(char **argv, t_shell_env *shell_env);
int							ft_export(char **args, t_shell_env *shell_env);
int							ft_unset(char **args, t_shell_env *shell_env);
int							ft_env(char **argv, t_shell_env *shell_env);
int							ft_export(char **args, t_shell_env *shell_env);
int							ft_exit(char **args, t_shell_env *shell);
t_builtin					*init_builtins(t_shell_env *shell_env);
int							exec_builtin(t_cmd *cmd, t_shell_env *shell_env);
int							is_builtin(t_shell_env *env, char *cmd_name);
int							ft_cd(char **args, t_shell_env *shell_env);
char						*get_cd_path(char **args, t_shell_env *shell_env);
int							check_cd_args(char **args, t_shell_env *shell_env);
int							validate_cd_path(char *path,
								t_shell_env *shell_env);
int							print_cd_error(char *path,
								char *error_msg, t_shell_env *shell_env);
void						update_env_pwd(t_shell_env *shell_env,
								char *new_pwd);

// external ls-cat-...etc
int							exec_external(t_cmd *cmd, t_shell_env *shell_env);
char						*resolve_path(char *cmd, char **env);
char						*search_in_path(char *cmd, char *path_env);
void						update_exit_status(t_shell_env *shell_env,
								int status);

// exec_cmds_utils

int							fork_and_exec(t_cmd *cmd,
								t_shell_env *env, t_pipe_info *pipes);
void						child_process(t_cmd *cmd,
								t_shell_env *env, t_pipe_info *pipes);
int							parent_process(pid_t pid, t_pipe_info *pipes,
								t_cmd *cmd, t_shell_env *env);
void						reset_std(int saved_stdin, int saved_stdout);
void						close_prev_fd(int prev_fd);

// exit_utils
int							ft_is_numeric(const char *str);
long						ft_atol(const char *str, int *error);
void						print_exit_error(char *arg, char *msg);

#endif