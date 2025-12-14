#include "../includes/minirt.h"

t_ray	generate_ray(t_scene *scene, int x, int y)
{
	double		uv[2];
	t_vector	pixel_center;
	t_ray		ray;

	uv[0] = (2.0 * ((double)x / (scene->width - 1)) - 1.0)
		* scene->camera.viewport_width / 2.0;
	uv[1] = (1.0 - 2.0 * ((double)y / (scene->height - 1)))
		* scene->camera.viewport_height / 2.0;
	pixel_center = scene->camera.orientation;
	pixel_center = vec_add(pixel_center,
			vec_scale(scene->camera.right, uv[0]));
	pixel_center = vec_add(pixel_center,
			vec_scale(scene->camera.up, uv[1]));
	ray.origin = scene->camera.position;
	ray.direction = vec_normalize(pixel_center);
	return (ray);
}
