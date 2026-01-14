#include "minirt.h"

int	main(int argc, char **argv)
{
	t_scene	scene;

	if (argc != 2)
		error_exit("Usage: ./miniRT <scene.rt>");
	if (!check_file_extension(argv[1], ".rt"))
		error_exit("Scene file must have .rt extension");
	init_scene(&scene);
	if (!parse_scene(argv[1], &scene))
	{
		cleanup_scene(&scene);
		exit(1);
	}
	init_mlx(&scene);
	init_camera(&scene.camera);
	render_scene(&scene);
	mlx_loop(scene.mlx);
	cleanup_scene(&scene);
	return (0);
}
