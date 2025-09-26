# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ablabib <ablabib@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 00:00:00 by student           #+#    #+#              #
#    Updated: 2025/09/26 17:52:09 by ablabib          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME		= libft_test

# Directories
LIBFT_DIR		= ./libft
LIBFT_LIB		= $(LIBFT_DIR)/libft.a
OBJ_DIR			= ./object_files

# Source files
SRCS			= main.c

# Object files
OBJS			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Compiler and flags
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I$(LIBFT_DIR)

# Colors for output
GREEN		= \033[32m
YELLOW		= \033[33m
RED			= \033[31m
BLUE		= \033[34m
MAGENTA		= \033[35m
CYAN		= \033[36m
RESET		= \033[0m
BOLD		= \033[1m

# Default target
all: $(NAME)

# Build the test program
$(NAME): $(LIBFT_LIB) $(OBJS)
	@printf "$(CYAN)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)
	@printf "$(GREEN)$(BOLD)✅ $(NAME) compiled successfully!$(RESET)\n"

# Create object directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@printf "$(YELLOW)Compiling $<...$(RESET)\n"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build libft
$(LIBFT_LIB):
	@printf "$(BLUE)Building libft...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR)
	@printf "$(GREEN)✅ libft compiled successfully!$(RESET)\n"

# Clean object files
clean:
	@printf "$(RED)Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@printf "$(GREEN)✅ Clean completed!$(RESET)\n"

# Full clean (including executables)
fclean: clean
	@printf "$(RED)Full cleaning...$(RESET)\n"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@printf "$(GREEN)✅ Full clean completed!$(RESET)\n"

# Rebuild everything
re: fclean all

# Run tests
test: $(NAME)
	@printf "$(MAGENTA)$(BOLD)\n🧪 Running libft tests...\n$(RESET)"
	@./$(NAME)

# Run tests with valgrind (memory leak detection)
valgrind: $(NAME)
	@printf "$(MAGENTA)$(BOLD)\n🔍 Running tests with Valgrind...\n$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

# Debug build (with debug symbols and no optimization)
debug: CFLAGS += -g3 -O0 -DDEBUG
debug: fclean $(NAME)
	@printf "$(CYAN)$(BOLD)🐛 Debug build completed!$(RESET)\n"

# Help target
help:
	@printf "$(BOLD)$(CYAN)Libft Tester Makefile$(RESET)\n"
	@printf "$(YELLOW)Available targets:$(RESET)\n"
	@printf "  $(GREEN)all$(RESET)      - Build the test program (default)\n"
	@printf "  $(GREEN)clean$(RESET)    - Remove object files\n"
	@printf "  $(GREEN)fclean$(RESET)   - Remove object files and executable\n"
	@printf "  $(GREEN)re$(RESET)       - Rebuild everything\n"
	@printf "  $(GREEN)test$(RESET)     - Build and run tests\n"
	@printf "  $(GREEN)valgrind$(RESET) - Run tests with Valgrind\n"
	@printf "  $(GREEN)debug$(RESET)    - Build debug version\n"
	@printf "  $(GREEN)help$(RESET)     - Show this help message\n"

# display failed test only
test-fail: $(NAME)
	@printf "$(MAGENTA)$(BOLD)\n🧪 Showing only failed tests...\n$(RESET)"
	@./$(NAME) | grep -E "✗|SEGFAULT"


# Check if libft directory exists
check-libft:
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		printf "$(RED)$(BOLD)❌ Error: libft directory not found!$(RESET)\n"; \
		printf "$(YELLOW)Please ensure the libft directory exists in the current directory.$(RESET)\n"; \
		exit 1; \
	fi
	@if [ ! -f "$(LIBFT_DIR)/Makefile" ]; then \
		printf "$(RED)$(BOLD)❌ Error: libft/Makefile not found!$(RESET)\n"; \
		printf "$(YELLOW)Please ensure libft has its own Makefile.$(RESET)\n"; \
		exit 1; \
	fi

# Phony targets
.PHONY: all clean fclean re test valgrind debug help check-libft test-fail

# Make all targets depend on libft check
$(NAME): | check-libft