# Noms
NAME = minishell
LIBFT = libft/libft.a
LDFLAGS = -lreadline

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
		excec_built.c \
		external.c \
		external_helpers.c \
		exc_cmd.c \
		env.c \
		env_utils.c \
		env_utils2.c \
		signal.c \
		parse1_utils.c \
		parse2_utils.c \
		parse_helpers.c \
		proc_tokens.c \
		get_next_token.c \
		parse_cmd.c \
		parse_repl_var.c \
		redir.c \
		pipes.c \
		free.c \
		ft_echo.c \
		ft_env.c \
		ft_pwd.c \
		ft_export.c \
		ft_unset.c \
		ft_exit.c \
		ft_cd.c

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

.PHONY: all clean fclean re header