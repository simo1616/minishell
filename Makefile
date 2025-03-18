# Noms
NAME = minishell
LIBFT = libft/libft.a
LDFLAGS = -lreadline

# Debugger
DEBUGGER = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes --verbose --suppressions=ignore_readline_leaks.supp --log-file=valgrind-out.txt

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INC = -I ./libft/inc -I ./inc

# Couleurs pour la déco
GREEN   = \033[1;32m
CYAN    = \033[1;36m
RESET   = \033[0m

# Sources et objets
SRC_DIR = src/
SRCS 	= main.c \
		init_builtins.c \
		exec_built.c \
		external.c \
		external_helpers.c \
		exec_cmd.c \
		env.c \
		env_utils.c \
		env_utils2.c \
		signal.c \
		calcul_len.c \
		parse2_utils.c \
		parse_helpers.c \
		proc_tokens.c \
		get_next_token.c \
		get_n_t_helper.c \
		parse_cmd.c \
		parse_repl_var.c \
		redir.c \
		ft_redirects.c \
		exec_pipes.c \
		free.c \
		ft_echo.c \
		ft_env.c \
		ft_pwd.c \
		ft_export.c \
		ft_unset.c \
		ft_exit.c \
		ft_cd.c \
		exec_cmd_utils.c \
		exec_pipes_utils.c \
		ft_cd_utils.c \
		ft_exit_utils.c \
		ft_export_utils.c \
		ft_redirects_utils.c

OBJS = $(addprefix $(SRC_DIR), $(SRCS:.c=.o))

# Règles
all: header $(LIBFT) $(NAME)


# Affichage du header
header:
	@echo "$(GREEN)    by         $(CYAN)@jdecarro$(GREEN)"
	@echo "::::    ::::  ::::::::::: ::::    ::: ::::::::::: ::::::::  :::    ::: :::::::::: :::        :::"
	@echo "+:+:+: :+:+:+     :+:     :+:+:   :+:     :+:    :+:    :+: :+:    :+: :+:        :+:        :+:"
	@echo "+:+ +:+:+ +:+     +:+     :+:+:+  +:+     +:+    +:+        +:+    +:+ +:+        +:+        +:+"
	@echo "+#+  +:+  +#+     +#+     +#+ +:+ +#+     +#+    +#++:++#++ +#++:++#++ +#++:++#   +#+        +#+"
	@echo "+#+       +#+     +#+     +#+  +#+#+#     +#+           +#+ +#+    +#+ +#+        +#+        +#+"
	@echo "#+#       #+#     #+#     #+#   #+#+#     #+#    #+#    #+# #+#    #+# #+#        #+#        #+#"
	@echo "###       ###     #+#     ###    #### ########### ########  ###    ### ########## ########## ##########"
	@echo "###       ### ########### ###    ####            $(CYAN)&$(GREEN)          ###    ###  $(CYAN)@mbendidi$(GREEN) ########## ##########"
	@echo "$(RESET)"


# Compilation de libft
$(LIBFT):
	@make -C libft

# Compilation de minishell
$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -I ./libft/inc -I ./inc -L./libft -lft $(LDFLAGS) -o $(NAME)

# Compilation des .o
$(SRC_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@make -C libft clean
	rm -f $(OBJS)
	@echo "$(CYAN)Clean completed!$(RESET)"

fclean: clean
	@make -C libft fclean
	rm -f $(NAME)

re: fclean all

debug: all
	$(DEBUGGER) ./$(NAME)

.PHONY: all clean fclean re header