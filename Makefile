NAME		= miniRT

OBJ_DIR		= obj

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I./includes -I./lib/libft -I./lib/MLX42/include
LDFLAGS		= -L./lib/libft -lft -L./lib/MLX42/build -lmlx42 -lglfw -lm -ldl -pthread

# Source files
SRCS		= src/core/main.c \
		  src/scene/init_scene.c \
		  src/core/cleanup.c \
		  src/core/error.c \
		  src/parser/parse_scene.c \
		  src/parser/parse_elements.c \
		  src/parser/parse_objects.c \
		  src/parser/parse_sphere.c \
		  src/parser/parse_utils.c \
		  src/parser/ft_atod.c \
		  src/parser/validate.c \
		  src/math/vec_basic.c \
		  src/math/vec_products.c \
		  src/math/vec_utils.c \
		  src/render/render.c \
		  src/render/ray_trace.c \
		  src/scene/camera.c \
		  src/shapes/intersect_sphere.c \
		  src/shapes/intersect_plane.c \
		  src/shapes/intersect_cylinder.c \
		  src/shapes/cylinder_utils.c \
		  src/render/lighting.c \
		  src/render/shadows.c \
		  src/utils/color.c \
		  src/mlx/mlx_init.c \
		  src/mlx/mlx_events.c \
		  src/mlx/mlx_utils.c

OBJS		= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Bonus source files
BONUS_SRCS	= src/core/main_bonus.c \
		  src/core/cleanup_bonus.c \
		  src/core/error_bonus.c \
		  src/scene/init_scene_bonus.c \
		  src/scene/camera_bonus.c \
		  src/parser/parse_scene_bonus.c \
		  src/parser/parse_elements_bonus.c \
		  src/parser/parse_objects_bonus.c \
		  src/parser/parse_sphere_bonus.c \
		  src/parser/parse_utils_bonus.c \
		  src/parser/ft_atod_bonus.c \
		  src/parser/validate_bonus.c \
		  src/parser/parse_cone_bonus.c \
		  src/math/vec_basic_bonus.c \
		  src/math/vec_products_bonus.c \
		  src/math/vec_utils_bonus.c \
		  src/math/vec_reflect_bonus.c \
		  src/render/render_bonus.c \
		  src/render/ray_trace_bonus.c \
		  src/render/lighting_bonus.c \
		  src/render/shadows_bonus.c \
		  src/render/specular_bonus.c \
		  src/shapes/intersect_sphere_bonus.c \
		  src/shapes/intersect_plane_bonus.c \
		  src/shapes/intersect_cylinder_bonus.c \
		  src/shapes/cylinder_utils_bonus.c \
		  src/shapes/intersect_cone_bonus.c \
		  src/shapes/cone_utils_bonus.c \
		  src/utils/color_bonus.c \
		  src/mlx/mlx_init_bonus.c \
		  src/mlx/mlx_events_bonus.c \
		  src/mlx/mlx_utils_bonus.c

BONUS_OBJS	= $(patsubst %.c,$(OBJ_DIR)/%.o,$(BONUS_SRCS))

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

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)/*
	@make -C lib/libft clean

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C lib/libft fclean
	@rm -rf lib/MLX42/build

re: fclean all

# Bonus build
bonus: libmlx libft $(BONUS_OBJS)
	@echo "$(GREEN)Linking $(NAME) (bonus)...$(RESET)"
	@$(CC) $(BONUS_OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) bonus created successfully!$(RESET)"

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

# Run leak tests with valgrind (all error paths + valid scenes)
test-leak: $(NAME)
	@chmod +x tests/run_tests.sh
	@./tests/run_tests.sh leak

# Run valgrind on a specific scene
valgrind: $(NAME)
	@if [ -z "$(SCENE)" ]; then \
		echo "$(RED)Usage: make valgrind SCENE=path/to/scene.rt$(RESET)"; \
		echo "  Or use: make test-leak  (runs all leak tests)"; \
	else \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--track-fds=yes --error-exitcode=42 ./$(NAME) $(SCENE); \
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

# =============================================================================
# Bonus test rules
# =============================================================================

test-bonus-step1: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh step1

test-bonus-step2: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh step2

test-bonus-step3: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh step3

test-bonus-step4: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh step4

test-bonus-step5: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh step5

test-bonus-all: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh all

test-bonus-compare:
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh compare

test-bonus-progress: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh progress

test-bonus-leak: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh leak

test-bonus-menu: bonus
	@chmod +x tests/test_bonus.sh
	@./tests/test_bonus.sh

.PHONY: all clean fclean re bonus libmlx libft test test1 test2 test3 test-error test-bonus test-edge test-menu test-visual test-leak run valgrind demo demo2 demo3 test-bonus-step1 test-bonus-step2 test-bonus-step3 test-bonus-step4 test-bonus-step5 test-bonus-all test-bonus-compare test-bonus-progress test-bonus-leak test-bonus-menu
