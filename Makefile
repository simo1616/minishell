# Noms
NAME = minishell
LIBFT = libft/libft.a

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INC = -I ./libft/inc -I ./inc

# Sources et objets
SRC_DIR = src/
SRCS = main.c \
       utils.c

OBJS = $(addprefix $(SRC_DIR), $(SRCS:.c=.o))

# Règles
all: $(LIBFT) $(NAME)

# Compilation de libft
$(LIBFT):
	@make -C libft

# Compilation de minishell
$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(INC) -L./libft -lft -o $(NAME)

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