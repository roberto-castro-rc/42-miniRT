#include "minirt.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		cleanup_scene(scene);
		exit(0);
	}
}

void	close_hook(void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	cleanup_scene(scene);
	exit(0);
}
