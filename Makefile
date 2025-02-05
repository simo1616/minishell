# Noms
NAME = minishell
LIBFT = libft/libft.a
LDFLAGS = -lreadline

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INC = -I ./libft/inc -I ./inc

# Sources et objets
SRC_DIR = src/
SRCS 	= main.c \
		init_builtins.c \
		excec_built.c \
		exc_cmd.c \
		env.c \
		env_utils.c \
		signal.c \
		parse.c \
		free.c \
		ft_echo.c \
		ft_env.c \
		ft_pwd.c \
		ft_export.c

OBJS = $(addprefix $(SRC_DIR), $(SRCS:.c=.o))

# Règles
all: $(LIBFT) $(NAME)

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

fclean: clean
	@make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re