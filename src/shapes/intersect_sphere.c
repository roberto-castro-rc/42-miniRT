#include "minirt.h"

static t_hit	create_no_hit(void)
{
	t_hit	hit;

	hit.hit = 0;
	hit.t = INFINITY;
	return (hit);
}

static double	solve_quadratic(double a, double b, double disc)
{
	double	t1;
	double	t2;
	double	sqrt_disc;

	sqrt_disc = sqrt(disc);
	t1 = (-b - sqrt_disc) / (2.0 * a);
	t2 = (-b + sqrt_disc) / (2.0 * a);
	if (t1 > EPSILON)
		return (t1);
	if (t2 > EPSILON)
		return (t2);
	return (-1.0);
}

t_hit	intersect_sphere(t_ray ray, t_sphere sp, double closest)
{
	t_vector	oc;
	double		abc[3];
	double		discriminant;
	double		t;
	t_hit		hit;

	oc = vec_subtract(ray.origin, sp.center);
	abc[0] = vec_dot(ray.direction, ray.direction);
	abc[1] = 2.0 * vec_dot(oc, ray.direction);
	abc[2] = vec_dot(oc, oc) - sp.radius * sp.radius;
	discriminant = abc[1] * abc[1] - 4.0 * abc[0] * abc[2];
	if (discriminant < 0)
		return (create_no_hit());
	t = solve_quadratic(abc[0], abc[1], discriminant);
	if (t < 0 || t >= closest)
		return (create_no_hit());
	hit.hit = 1;
	hit.t = t;
	hit.point = ray_at(ray, t);
	hit.normal = vec_normalize(vec_subtract(hit.point, sp.center));
	hit.color = sp.color;
	return (hit);
}
