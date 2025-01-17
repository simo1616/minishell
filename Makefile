# Noms
NAME = minishell
LIBFT = libft/libft.a

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INC = -I ./libft/inc

# Sources et objets
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Règles
all: $(LIBFT) $(NAME)

# Compilation de libft (qui inclut gnl et printf)
$(LIBFT):
	@make -C libft

# Compilation de minishell
$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(INC) -L./libft -lft -o $(NAME)

# Compilation des .o
%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@make -C libft clean
	rm -f $(OBJS)

fclean: clean
	@make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re