#include "minirt.h"

double	vec_magnitude(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vector	vec_normalize(t_vector v)
{
	double		mag;
	t_vector	result;

	mag = vec_magnitude(v);
	if (mag < EPSILON)
		return (v);
	result.x = v.x / mag;
	result.y = v.y / mag;
	result.z = v.z / mag;
	return (result);
}

double	vec_distance(t_vector a, t_vector b)
{
	t_vector	diff;

	diff = vec_subtract(a, b);
	return (vec_magnitude(diff));
}

t_ray	ray_create(t_vector origin, t_vector direction)
{
	t_ray	ray;

	ray.origin = origin;
	ray.direction = vec_normalize(direction);
	return (ray);
}

t_vector	ray_at(t_ray ray, double t)
{
	t_vector	result;

	result = vec_add(ray.origin, vec_scale(ray.direction, t));
	return (result);
}
