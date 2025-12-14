#include "../includes/minirt.h"

static void	check_spheres(t_scene *scene, t_ray ray, t_hit *closest)
{
	int		i;
	t_hit	hit;

	i = 0;
	while (i < scene->sphere_count)
	{
		hit = intersect_sphere(ray, scene->spheres[i], closest->t);
		if (hit.hit && hit.t < closest->t)
			*closest = hit;
		i++;
	}
}

static void	check_planes(t_scene *scene, t_ray ray, t_hit *closest)
{
	int		i;
	t_hit	hit;

	i = 0;
	while (i < scene->plane_count)
	{
		hit = intersect_plane(ray, scene->planes[i], closest->t);
		if (hit.hit && hit.t < closest->t)
			*closest = hit;
		i++;
	}
}

static void	check_cylinders(t_scene *scene, t_ray ray, t_hit *closest)
{
	int		i;
	t_hit	hit;

	i = 0;
	while (i < scene->cylinder_count)
	{
		hit = intersect_cylinder(ray, scene->cylinders[i], closest->t);
		if (hit.hit && hit.t < closest->t)
			*closest = hit;
		i++;
	}
}

t_hit	find_closest_hit(t_scene *scene, t_ray ray)
{
	t_hit	closest;

	closest.hit = 0;
	closest.t = INFINITY;
	check_spheres(scene, ray, &closest);
	check_planes(scene, ray, &closest);
	check_cylinders(scene, ray, &closest);
	return (closest);
}

t_color	trace_ray(t_scene *scene, int x, int y)
{
	t_ray	ray;
	t_hit	hit;
	t_color	color;

	ray = generate_ray(scene, x, y);
	hit = find_closest_hit(scene, ray);
	if (!hit.hit)
		return ((t_color){0, 0, 0});
	color = calculate_lighting(scene, hit, ray);
	return (color);
}
