#include "minirt_bonus.h"

static void	calc_cone_abc(t_ray ray, t_cone cn, double *abc)
{
	t_vector	oc;
	double		dv;
	double		ocv;
	double		cos2;

	oc = vec_subtract(ray.origin, cn.center);
	dv = vec_dot(ray.direction, cn.axis);
	ocv = vec_dot(oc, cn.axis);
	cos2 = cos(cn.half_angle) * cos(cn.half_angle);
	abc[0] = dv * dv - cos2 * vec_dot(ray.direction, ray.direction);
	abc[1] = 2.0 * (dv * ocv - cos2 * vec_dot(ray.direction, oc));
	abc[2] = ocv * ocv - cos2 * vec_dot(oc, oc);
}

static void	sort_roots(double *t, double *disc)
{
	if (t[0] > t[1])
	{
		*disc = t[0];
		t[0] = t[1];
		t[1] = *disc;
	}
}

static double	check_cone_hit(t_ray ray, t_cone cn, double closest)
{
	double	abc[3];
	double	disc;
	double	t[2];

	calc_cone_abc(ray, cn, abc);
	disc = abc[1] * abc[1] - 4.0 * abc[0] * abc[2];
	if (disc < 0 || fabs(abc[0]) < EPSILON)
		return (-1.0);
	t[0] = (-abc[1] - sqrt(disc)) / (2.0 * abc[0]);
	t[1] = (-abc[1] + sqrt(disc)) / (2.0 * abc[0]);
	sort_roots(t, &disc);
	if (check_cone_body(ray, cn, t[0], closest))
		return (t[0]);
	if (check_cone_body(ray, cn, t[1], closest))
		return (t[1]);
	return (-1.0);
}

static t_hit	best_hit(t_hit a, t_hit b)
{
	if (!a.hit)
		return (b);
	if (!b.hit)
		return (a);
	if (a.t <= b.t)
		return (a);
	return (b);
}

t_hit	intersect_cone(t_ray ray, t_cone cn, double closest)
{
	t_hit		body;
	t_hit		cap;
	t_vector	base;
	double		t_body;

	t_body = check_cone_hit(ray, cn, closest);
	body = create_no_hit();
	if (t_body > 0)
	{
		body = create_cone_body_hit(ray, cn, t_body);
		closest = t_body;
	}
	base = vec_add(cn.center, vec_scale(cn.axis, cn.height));
	cap = check_cone_cap(ray, cn, base, closest);
	return (best_hit(body, cap));
}
