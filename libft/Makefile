# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbendidi <mbendidi@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/17 20:56:21 by mbendidi          #+#    #+#              #
#    Updated: 2024/12/25 17:38:17 by mbendidi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL       = /bin/bash

NAME        = libft.a
INCLUDE     = inc/
SRC_DIR     = src/
OBJ_DIR     = obj/
CC          = gcc
CFLAGS      = -Wall -Werror -Wextra -I
RM          = rm -f
AR          = ar rcs

FTIS_DIR    = ft_is/
FTIS        = ft_isalnum ft_isalpha ft_isascii ft_isdigit ft_isprint

FTMEM_DIR   = ft_mem/
FTMEM       = ft_bzero ft_calloc ft_memchr ft_memcmp ft_memcpy ft_memmove ft_memset

FTPUT_DIR   = ft_put/
FTPUT       = ft_putchar_fd ft_putendl_fd ft_putnbr_fd ft_putstr_fd

FTTO_DIR    = ft_to/
FTTO        = ft_atoi ft_itoa ft_tolower ft_toupper

FTSTR_DIR   = ft_str/
FTSTR       = ft_split ft_strchr ft_strdup ft_striteri ft_strjoin \
              ft_strlcat ft_strlcpy ft_strlen ft_strmapi ft_strncmp \
              ft_strnstr ft_strrchr ft_strtrim ft_substr ft_strcmp

FTLST_DIR   = ft_lst/
FTLST       = ft_lstadd_back ft_lstadd_front ft_lstclear ft_lstdelone \
              ft_lstiter ft_lstlast ft_lstmap ft_lstnew ft_lstsize

GNL_DIR     = gnl/
GNL         = get_next_line get_next_line_utils

FTPRINTF_DIR= ft_printf/
FTPRINTF    = ft_printf ft_printf_utils ft_print_hex ft_print_ptr ft_print_unsigned

SRC_FILES += $(addprefix $(FTIS_DIR),$(FTIS))
SRC_FILES += $(addprefix $(FTMEM_DIR),$(FTMEM))
SRC_FILES += $(addprefix $(FTPUT_DIR),$(FTPUT))
SRC_FILES += $(addprefix $(FTTO_DIR),$(FTTO))
SRC_FILES += $(addprefix $(FTSTR_DIR),$(FTSTR))
SRC_FILES += $(addprefix $(FTLST_DIR),$(FTLST))
SRC_FILES += $(addprefix $(GNL_DIR),$(GNL))
SRC_FILES += $(addprefix $(FTPRINTF_DIR),$(FTPRINTF))

SRC         = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ         = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

TOTAL = $(words $(SRC_FILES))
COUNT = 0
BAR_WIDTH = 40

OBJF        = .cache_exists

all: $(NAME)

$(NAME): $(OBJ)
	@$(AR) $(NAME) $(OBJ)
	@printf "\r\033[K%.*s100%% libft compiled\n" $(BAR_WIDTH) "========================================"

$(OBJ_DIR)%.o : $(SRC_DIR)%.c | $(OBJF)
	@$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
	@(\
		PCT=$$((100*$(COUNT)/$(TOTAL))); \
		BAR_LEN=$$(( $(BAR_WIDTH)*$(COUNT)/$(TOTAL) )); \
		printf "\r\033[K%.*s%d%%" $$BAR_LEN "========================================" $$PCT; \
	)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJF):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(FTIS_DIR)
	@mkdir -p $(OBJ_DIR)$(FTMEM_DIR)
	@mkdir -p $(OBJ_DIR)$(FTPUT_DIR)
	@mkdir -p $(OBJ_DIR)$(FTTO_DIR)
	@mkdir -p $(OBJ_DIR)$(FTSTR_DIR)
	@mkdir -p $(OBJ_DIR)$(FTLST_DIR)
	@mkdir -p $(OBJ_DIR)$(GNL_DIR)
	@mkdir -p $(OBJ_DIR)$(FTPRINTF_DIR)
	@touch $(OBJF)

clean:
	@$(RM) -r $(OBJ_DIR)
	@$(RM) $(OBJF)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

norm:
	@norminette $(SRC) $(INCLUDE) | grep -v Norme -B1 || true

.PHONY: all clean fclean re norm
