# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/11 19:22:34 by rmakende          #+#    #+#              #
#    Updated: 2025/07/20 19:12:20 by ruortiz-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I./includes
RL_FLAGS = -lreadline

LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a
EXEC_DIR = ./execution
PARSING_DIR = ./parsing

EXEC_SRCS = $(EXEC_DIR)/main.c \
		$(EXEC_DIR)/builtins/builtins.c \
		$(EXEC_DIR)/builtins/builtins2.c \
		$(EXEC_DIR)/builtins/builtins_utils.c \
		$(EXEC_DIR)/path_utils.c \
		$(EXEC_DIR)/env/env.c \
		$(EXEC_DIR)/env/env_utils.c

PARSING_SRCS = $(PARSING_DIR)/parsing.c \
		$(PARSING_DIR)/quote_handler.c \
		$(PARSING_DIR)/token_utils.c \
		$(PARSING_DIR)/token_validation.c \
		$(PARSING_DIR)/variable_expansion.c

SRCS = $(EXEC_SRCS) $(PARSING_SRCS)

OBJ_DIR = obj
EXEC_OBJS = $(EXEC_SRCS:$(EXEC_DIR)/%.c=$(OBJ_DIR)/execution/%.o)
PARSING_OBJS = $(PARSING_SRCS:$(PARSING_DIR)/%.c=$(OBJ_DIR)/parsing/%.o)
OBJS = $(EXEC_OBJS) $(PARSING_OBJS)

NAME = minishell

GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[1;33m
NC = \033[0m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(NC)"
	@make -C $(LIBFT_DIR)
	@echo "$(GREEN)Libft compiled!$(NC)"

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(YELLOW)Compiling $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(RL_FLAGS) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)$(NAME) compiled!$(NC)"

# Reemplazar las reglas de compilación con una más genérica:
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Añadir regla para asegurar que los directorios existen
$(OBJS): | $(OBJ_DIR)/execution $(OBJ_DIR)/parsing $(OBJ_DIR)/execution/builtins $(OBJ_DIR)/execution/env

$(OBJ_DIR)/execution $(OBJ_DIR)/parsing $(OBJ_DIR)/execution/builtins $(OBJ_DIR)/execution/env:
	@mkdir -p $@

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(GREEN)Clean done!$(NC)"

fclean: clean
	@echo "$(RED)Full clean...$(NC)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)Full clean done!$(NC)"

re: fclean all

run: all
	./$(NAME)

.PHONY: all clean fclean re run