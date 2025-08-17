# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/11 19:22:34 by rmakende          #+#    #+#              #
#    Updated: 2025/08/17 21:04:30 by ruortiz-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror  -I./includes #-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
RL_FLAGS = -lreadline

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
EXEC_DIR = ./execution
PARSING_DIR = ./parsing

EXEC_SRCS =	$(EXEC_DIR)/core/main.c \
		$(EXEC_DIR)/core/shell_init.c \
		$(EXEC_DIR)/core/shell_loop.c \
		$(EXEC_DIR)/core/signals/signals.c \
		$(EXEC_DIR)/core/signals/signals_utils.c \
		$(EXEC_DIR)/exec_system/pipeline/pipeline_executor.c \
		$(EXEC_DIR)/exec_system/pipeline/pipeline_helpers.c \
		$(EXEC_DIR)/exec_system/pipeline/pipeline_loop.c \
		$(EXEC_DIR)/exec_system/pipeline/pipeline_utils.c \
		$(EXEC_DIR)/exec_system/utils/error_utils.c \
		$(EXEC_DIR)/exec_system/utils/execution_wrappers.c \
		$(EXEC_DIR)/exec_system/utils/execution_processes.c \
		$(EXEC_DIR)/exec_system/processes/child_process.c \
		$(EXEC_DIR)/exec_system/processes/parent_process.c \
		$(EXEC_DIR)/exec_system/processes/exec_dispatch.c \
		$(EXEC_DIR)/exec_system/processes/exec_single.c \
		$(EXEC_DIR)/exec_system/commands/command_executor.c \
		$(EXEC_DIR)/exec_system/commands/path_resolver.c \
		$(EXEC_DIR)/redirection/redirect.c \
		$(EXEC_DIR)/redirection/redirect_heredoc.c \
		$(EXEC_DIR)/redirection/multiple_redirections.c \
		$(EXEC_DIR)/redirection/validators/redirection_validator.c \
		$(EXEC_DIR)/redirection/validators/validator_utils.c \
		$(EXEC_DIR)/pipe_system/pipe_handler.c \
		$(EXEC_DIR)/pipe_system/setup_pipes.c \
		$(EXEC_DIR)/builtins/core/builtins.c \
		$(EXEC_DIR)/builtins/core/builtins_utils.c \
		$(EXEC_DIR)/builtins/core/env/env.c \
		$(EXEC_DIR)/builtins/core/env/env_utils.c \
		$(EXEC_DIR)/builtins/commands/cd/builtin_cd.c \
		$(EXEC_DIR)/builtins/commands/cd/cd_helpers.c \
		$(EXEC_DIR)/builtins/commands/echo_unset_env.c \
		$(EXEC_DIR)/builtins/commands/exit.c

PARSING_SRCS = $(PARSING_DIR)/parsing_utils.c \
		$(PARSING_DIR)/expand_tokens.c \
		$(PARSING_DIR)/token_validation.c \
		$(PARSING_DIR)/variable_expansion.c \
		$(PARSING_DIR)/variable_expansion_complex.c \
		$(PARSING_DIR)/variable_expansion_core.c \
		$(PARSING_DIR)/variable_expansion_helpers.c \
		$(PARSING_DIR)/variable_expansion_utils.c \
		$(PARSING_DIR)/tokens/token_parser.c \
		$(PARSING_DIR)/tokens/token_parser_utils.c \
		$(PARSING_DIR)/tokens/argv_parser.c \
		$(PARSING_DIR)/tokens/redirection_parser.c \
		$(PARSING_DIR)/tokens/unified_parser/unified_parser.c \
		$(PARSING_DIR)/tokens/unified_parser/unified_parser_utils.c \
		$(PARSING_DIR)/tokens/token_cleanup.c \
		$(PARSING_DIR)/tokens/token_creation.c \
		$(PARSING_DIR)/lexer/token_utils.c \
		$(PARSING_DIR)/lexer/string_utils.c \
		$(PARSING_DIR)/lexer/buffer_handler.c \
		$(PARSING_DIR)/lexer/operator_handler.c \
		$(PARSING_DIR)/lexer/quote_handler.c \
		$(PARSING_DIR)/lexer/handler_helper.c \
		$(PARSING_DIR)/lexer/tokenizer_main.c \
		$(PARSING_DIR)/lexer/tokenizer_context.c \
		$(PARSING_DIR)/lexer/tokenizer_utils.c

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

animate:
	@clear

	@echo "     \033[1;37m _______________________________________________________"
	@echo "     .'  __________________________________________________________  '."
	@echo "     : .'                                                          '. :"
	@echo "     | |      ________________________________________________      | |"
	@echo "     | |    .:________________________________________________:.    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |                                                  |    | |"
	@echo "     | |    |            __________________________            |    | |"
	@echo "     | |    |           |  |  |  |  |  |  |  |  |  |           |    | |"
	@echo "     | |    '.__________|__|__|__|__|__|__|__|__|__|__________.'    | |"
	@echo "     | |                                                            | |"
	@echo "     | |                            iMac                            | |"
	@echo "     : '.__________________________________________________________.' :"
	@echo "      \".____________________________\\__/____________________________.\""
	@echo "                                     ||"
	@echo "                                     ||"
	@echo "                                     ||"
	@echo "                                  ___||___"
	@echo "                            _.--\"\"   \"\"   \"\"--._"
	@echo "                         .'\"       .-L-.        \"'."
	@echo "                       .'          : _ (           '."
	@echo "                     .'             \" \"              '."
	@echo "                    .'                                '."
	@echo "                    :         ________________         :"
	@echo "                   .'       .'                '.       '."
	@echo "                   :        '.________________.'        :"
	@echo "                   |----......______    ______......----|"
	@echo "                   :                \"\"\"\"                :"
	@echo "                   '.                                  .'"
	@echo "                     \"-.____. . . . . . . . . . ____.-\""
	@echo "                            \"\"\"\"\"\"--------\"\"\"\"\"\""      ""

	@sleep 1
	@tput cup 10 20; echo "████████████████████████████████████████"
	@tput cup 11 20; echo "█▌                                     █"
	@tput cup 12 20; echo "█▌   doing some scripts...             █"
	@tput cup 13 20; echo "█▌                                     █"
	@tput cup 14 20; echo "████████████████████████████████████████"
	@tput cup 15 20; echo ""
	@tput cup 15 20; echo ""
	@tput cup 15 20; echo ""
	@echo "\t"


	@sleep 2
	@clear

	@tput cup 3 5; echo "╔╗ ┬┌─┐┌┐┌┬  ┬┌─┐┌┐┌┬┌┬┐┌─┐  ┌─┐  ┬  ┌─┐  ┌┬┐┬┌┐┌┬┌─┐┬ ┬┌─┐┬  ┬    ┌┬┐┌─┐"
	@tput cup 4 5; echo "╠╩╗│├┤ │││└┐┌┘├┤ ││││ │││ │  ├─┤  │  ├─┤  ││││││││└─┐├─┤├┤ │  │     ││├┤"
	@tput cup 5 5; echo "╚═╝┴└─┘┘└┘ └┘ └─┘┘└┘┴─┴┘└─┘  ┴ ┴  ┴─┘┴ ┴  ┴ ┴┴┘└┘┴└─┘┴ ┴└─┘┴─┘┴─┘  ─┴┘└─┘"
	@tput cup 6 5; echo "      \033[1;32m┌┬┐┌─┐┬┌─┌─┐┌┐┌  ┬ ┬  ┬─┐┬ ┬┌┐ ┌─┐┌┐┌\033[0m"
	@tput cup 7 5; echo "~~~~~~\033[1;32m│││├─┤├┴┐├┤ │││  └┬┘  ├┬┘│ │├┴┐├┤ │││\033[0m~~~~~~~"
	@tput cup 8 5; echo "      \033[1;32m┴ ┴┴ ┴┴ ┴└─┘┘└┘   ┴   ┴└─└─┘└─┘└─┘┘└┘\033[0m"


$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(NC)"
	@$(MAKE) -sC $(LIBFT_DIR)
	@echo "$(GREEN)Libft compiled!$(NC)"

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(YELLOW)Compiling $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(RL_FLAGS) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)$(NAME) compiled!$(NC)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): | $(OBJ_DIR)/execution $(OBJ_DIR)/parsing $(OBJ_DIR)/execution/core $(OBJ_DIR)/execution/core/signals $(OBJ_DIR)/execution/exec_system $(OBJ_DIR)/execution/exec_system/pipeline $(OBJ_DIR)/execution/exec_system/utils $(OBJ_DIR)/execution/exec_system/processes $(OBJ_DIR)/execution/exec_system/commands $(OBJ_DIR)/execution/redirection $(OBJ_DIR)/execution/redirection/validators $(OBJ_DIR)/execution/pipe_system $(OBJ_DIR)/execution/builtins $(OBJ_DIR)/execution/builtins/core $(OBJ_DIR)/execution/builtins/core/env $(OBJ_DIR)/execution/builtins/commands $(OBJ_DIR)/execution/builtins/commands/cd $(OBJ_DIR)/parsing/tokens $(OBJ_DIR)/parsing/tokens/unified_parser $(OBJ_DIR)/parsing/lexer

$(OBJ_DIR)/execution $(OBJ_DIR)/parsing $(OBJ_DIR)/execution/core $(OBJ_DIR)/execution/core/signals $(OBJ_DIR)/execution/exec_system $(OBJ_DIR)/execution/exec_system/pipeline $(OBJ_DIR)/execution/exec_system/utils $(OBJ_DIR)/execution/exec_system/processes $(OBJ_DIR)/execution/exec_system/commands $(OBJ_DIR)/execution/redirection $(OBJ_DIR)/execution/redirection/validators $(OBJ_DIR)/execution/pipe_system $(OBJ_DIR)/execution/builtins $(OBJ_DIR)/execution/builtins/core $(OBJ_DIR)/execution/builtins/core/env $(OBJ_DIR)/execution/builtins/commands $(OBJ_DIR)/execution/builtins/commands/cd $(OBJ_DIR)/parsing/tokens $(OBJ_DIR)/parsing/tokens/unified_parser $(OBJ_DIR)/parsing/lexer:
	@mkdir -p $@

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -sC $(LIBFT_DIR) clean
	@echo "$(GREEN)Clean done!$(NC)"

fclean: clean
	@echo "$(RED)Full clean...$(NC)"
	@rm -f $(NAME)
	@rm -f *.txt
	@$(MAKE) -sC $(LIBFT_DIR) fclean
	@echo "$(GREEN)Full clean done!$(NC)"

re: fclean all

run: all
	@./$(NAME)

start: all
	@echo "$(YELLOW)entering minishell...$(NC)"
	@./$(NAME)

.PHONY: all clean fclean re run start animate

