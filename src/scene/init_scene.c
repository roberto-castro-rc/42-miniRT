#include "minirt.h"

void	init_scene(t_scene *scene)
{
	scene->ambient.ratio = 0.0;
	scene->ambient.color = (t_color){0, 0, 0};
	scene->lights = NULL;
	scene->light_count = 0;
	scene->spheres = NULL;
	scene->sphere_count = 0;
	scene->planes = NULL;
	scene->plane_count = 0;
	scene->cylinders = NULL;
	scene->cylinder_count = 0;
	scene->mlx = NULL;
	scene->image = NULL;
	scene->width = WIDTH;
	scene->height = HEIGHT;
}

void	init_camera(t_camera *cam)
{
	double		theta;
	t_vector	world_up;

	cam->aspect_ratio = (double)WIDTH / (double)HEIGHT;
	theta = cam->fov * M_PI / 180.0;
	cam->viewport_height = 2.0 * tan(theta / 2.0);
	cam->viewport_width = cam->viewport_height * cam->aspect_ratio;
	world_up = vec_create(0, 1, 0);
	cam->right = vec_normalize(vec_cross(cam->orientation, world_up));
	cam->up = vec_normalize(vec_cross(cam->right, cam->orientation));
}
