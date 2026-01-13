NAME		= miniRT

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I./includes -I./lib/libft -I./lib/MLX42/include
LDFLAGS		= -L./lib/libft -lft -L./lib/MLX42/build -lmlx42 -lglfw -lm -ldl -pthread

# Source files
SRCS		= src/main.c \
			  src/init_scene.c \
			  src/cleanup.c \
			  src/error.c \
			  src/parse_scene.c \
			  src/parse_elements.c \
			  src/parse_objects.c \
			  src/parse_sphere.c \
			  src/parse_utils.c \
			  src/validate.c \
			  src/vec_basic.c \
			  src/vec_products.c \
			  src/vec_utils.c \
			  src/render.c \
			  src/ray_trace.c \
			  src/camera.c \
			  src/intersect_sphere.c \
			  src/intersect_plane.c \
			  src/intersect_cylinder.c \
			  src/cylinder_utils.c \
			  src/lighting.c \
			  src/shadows.c \
			  src/color.c \
			  src/mlx_init.c \
			  src/mlx_events.c \
			  src/mlx_utils.c

OBJS		= $(SRCS:.c=.o)

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

all: libmlx libft $(NAME)

libmlx:
	@if [ ! -d "lib/MLX42/build" ]; then \
		echo "$(GREEN)Building MLX42...$(RESET)"; \
		cmake lib/MLX42 -B lib/MLX42/build && make -C lib/MLX42/build -j4; \
	fi

libft:
	@echo "$(GREEN)Building libft...$(RESET)"
	@make -C lib/libft

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@make -C lib/libft clean

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C lib/libft fclean
	@rm -rf lib/MLX42/build

re: fclean all

# =============================================================================
# Test rules
# =============================================================================

# Run all tests
test: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh all

# Run Level 1 tests (basic)
test1: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh level1

# Run Level 2 tests (intermediate)
test2: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh level2

# Run Level 3 tests (advanced)
test3: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh level3

# Run error handling tests
test-error: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh error

# Run bonus tests (multiple lights)
test-bonus: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh bonus

# Run edge case tests
test-edge: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh edge

# Interactive test menu
test-menu: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh

# Quick visual test
test-visual: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh visual

# Run specific test file
run: $(NAME)
	@if [ -z "$(SCENE)" ]; then \
		echo "$(RED)Usage: make run SCENE=path/to/scene.rt$(RESET)"; \
	else \
		./$(NAME) $(SCENE); \
	fi

# Run with valgrind (memory check)
valgrind: $(NAME)
	@if [ -z "$(SCENE)" ]; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) scenes/simple.rt; \
	else \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(SCENE); \
	fi

# Quick test scenes
demo: $(NAME)
	@echo "$(GREEN)Running demo scenes...$(RESET)"
	@./$(NAME) tests/level1_basic/01_single_sphere.rt

demo2: $(NAME)
	@echo "$(GREEN)Running intermediate demo...$(RESET)"
	@./$(NAME) tests/level2_intermediate/03_mixed_objects.rt

demo3: $(NAME)
	@echo "$(GREEN)Running advanced demo...$(RESET)"
	@./$(NAME) tests/level3_advanced/02_snowman.rt

.PHONY: all clean fclean re libmlx libft test test1 test2 test3 test-error test-bonus test-edge test-menu test-visual run valgrind demo demo2 demo3
