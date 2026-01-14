#include "minirt.h"

void	init_mlx(t_scene *scene)
{
	scene->mlx = mlx_init(scene->width, scene->height, "miniRT", true);
	if (!scene->mlx)
		error_exit("Failed to initialize MLX42");
	scene->image = mlx_new_image(scene->mlx, scene->width, scene->height);
	if (!scene->image)
	{
		mlx_terminate(scene->mlx);
		error_exit("Failed to create image");
	}
	if (mlx_image_to_window(scene->mlx, scene->image, 0, 0) < 0)
	{
		mlx_delete_image(scene->mlx, scene->image);
		mlx_terminate(scene->mlx);
		error_exit("Failed to put image to window");
	}
	mlx_key_hook(scene->mlx, key_hook, scene);
	mlx_close_hook(scene->mlx, close_hook, scene);
}
