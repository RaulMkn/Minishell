# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/11 19:22:34 by rmakende          #+#    #+#              #
#    Updated: 2025/07/11 20:18:26 by rmakende         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
RL_FLAGS = -lreadline

LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a
EXEC_DIR = ./execution

SRCS = $(EXEC_DIR)/main.c $(EXEC_DIR)/env.c $(EXEC_DIR)/builtins.c $(EXEC_DIR)/path_utils.c
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

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

$(OBJ_DIR)/%.o: $(EXEC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

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