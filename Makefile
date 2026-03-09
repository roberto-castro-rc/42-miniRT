NAME		= miniRT

OBJ_DIR		= obj

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I./includes -I./lib/libft -I./lib/MLX42/include
LDFLAGS		= -L./lib/libft -lft -L./lib/MLX42/build -lmlx42 -lglfw -lm -ldl -pthread

# Source files
SRCS		= src/mandatory/core/main.c \
		  src/mandatory/scene/init_scene.c \
		  src/mandatory/core/cleanup.c \
		  src/mandatory/core/error.c \
		  src/mandatory/parser/parse_scene.c \
		  src/mandatory/parser/parse_scene_helpers.c \
		  src/mandatory/parser/parse_elements.c \
		  src/mandatory/parser/parse_objects.c \
		  src/mandatory/parser/parse_sphere.c \
		  src/mandatory/parser/parse_utils.c \
		  src/mandatory/parser/ft_atod.c \
		  src/mandatory/parser/validate.c \
		  src/mandatory/math/vec_basic.c \
		  src/mandatory/math/vec_products.c \
		  src/mandatory/math/vec_utils.c \
		  src/mandatory/render/render.c \
		  src/mandatory/render/ray_trace.c \
		  src/mandatory/scene/camera.c \
		  src/mandatory/shapes/intersect_sphere.c \
		  src/mandatory/shapes/intersect_plane.c \
		  src/mandatory/shapes/intersect_cylinder.c \
		  src/mandatory/shapes/cylinder_utils.c \
		  src/mandatory/render/lighting.c \
		  src/mandatory/render/shadows.c \
		  src/mandatory/utils/color.c \
		  src/mandatory/mlx/mlx_init.c \
		  src/mandatory/mlx/mlx_events.c \
		  src/mandatory/mlx/mlx_utils.c

OBJS		= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Bonus source files
BONUS_SRCS	= src/bonus/core/main_bonus.c \
		  src/bonus/core/cleanup_bonus.c \
		  src/bonus/core/error_bonus.c \
		  src/bonus/scene/init_scene_bonus.c \
		  src/bonus/scene/camera_bonus.c \
		  src/bonus/parser/parse_scene_bonus.c \
		  src/bonus/parser/parse_elements_bonus.c \
		  src/bonus/parser/parse_objects_bonus.c \
		  src/bonus/parser/parse_sphere_bonus.c \
		  src/bonus/parser/parse_utils_bonus.c \
		  src/bonus/parser/ft_atod_bonus.c \
		  src/bonus/parser/validate_bonus.c \
		  src/bonus/parser/parse_cone_bonus.c \
		  src/bonus/math/vec_basic_bonus.c \
		  src/bonus/math/vec_products_bonus.c \
		  src/bonus/math/vec_utils_bonus.c \
		  src/bonus/math/vec_reflect_bonus.c \
		  src/bonus/render/render_bonus.c \
		  src/bonus/render/ray_trace_bonus.c \
		  src/bonus/render/lighting_bonus.c \
		  src/bonus/render/shadows_bonus.c \
		  src/bonus/render/specular_bonus.c \
		  src/bonus/render/checkerboard_bonus.c \
		  src/bonus/render/bumpmap_bonus.c \
		  src/bonus/shapes/intersect_sphere_bonus.c \
		  src/bonus/shapes/intersect_plane_bonus.c \
		  src/bonus/shapes/intersect_cylinder_bonus.c \
		  src/bonus/shapes/cylinder_utils_bonus.c \
		  src/bonus/shapes/intersect_cone_bonus.c \
		  src/bonus/shapes/cone_utils_bonus.c \
		  src/bonus/utils/color_bonus.c \
		  src/bonus/mlx/mlx_init_bonus.c \
		  src/bonus/mlx/mlx_events_bonus.c \
		  src/bonus/mlx/mlx_utils_bonus.c

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
# Utility rules
# =============================================================================

# Run specific scene
run: $(NAME)
	@if [ -z "$(SCENE)" ]; then \
		echo "$(RED)Usage: make run SCENE=path/to/scene.rt$(RESET)"; \
	else \
		./$(NAME) $(SCENE); \
	fi

# Run valgrind on a specific scene
valgrind: $(NAME)
	@if [ -z "$(SCENE)" ]; then \
		echo "$(RED)Usage: make valgrind SCENE=path/to/scene.rt$(RESET)"; \
	else \
		valgrind --leak-check=full --show-leak-kinds=definite,indirect,possible --track-origins=yes \
		--track-fds=no --suppressions=mlx.supp ./$(NAME) $(SCENE); \
		echo "$(GREEN)Note: small leaks from GPU/system drivers (???) are not from miniRT$(RESET)"; \
	fi

norm:
	@norminette src/ includes/ lib/libft/

.PHONY: all clean fclean re bonus libmlx libft norm run valgrind
