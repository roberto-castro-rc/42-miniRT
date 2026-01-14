#include "minirt.h"

static double	check_body(t_ray ray, t_cylinder cy, double closest)
{
	t_vector	oc;
	t_vector	proj[2];
	double		abc[3];
	double		t[2];

	oc = vec_subtract(ray.origin, cy.center);
	proj[0] = vec_subtract(ray.direction,
			vec_scale(cy.axis, vec_dot(ray.direction, cy.axis)));
	proj[1] = vec_subtract(oc, vec_scale(cy.axis, vec_dot(oc, cy.axis)));
	abc[0] = vec_dot(proj[0], proj[0]);
	abc[1] = 2.0 * vec_dot(proj[1], proj[0]);
	abc[2] = vec_dot(proj[1], proj[1]) - cy.radius * cy.radius;
	abc[2] = abc[1] * abc[1] - 4.0 * abc[0] * abc[2];
	if (abc[2] < 0)
		return (-1.0);
	t[0] = (-abc[1] - sqrt(abc[2])) / (2.0 * abc[0]);
	t[1] = (-abc[1] + sqrt(abc[2])) / (2.0 * abc[0]);
	if (check_body_hit(ray, cy, t[0], closest))
		return (t[0]);
	if (check_body_hit(ray, cy, t[1], closest))
		return (t[1]);
	return (-1.0);
}

t_hit	intersect_cylinder(t_ray ray, t_cylinder cy, double closest)
{
	double		t_body;
	t_hit		hit_cap1;
	t_hit		hit_cap2;
	t_vector	top;

	t_body = check_body(ray, cy, closest);
	if (t_body > 0 && t_body < closest)
		closest = t_body;
	hit_cap1 = check_cap(ray, cy, cy.center, closest);
	if (hit_cap1.hit && hit_cap1.t < closest)
		closest = hit_cap1.t;
	top = vec_add(cy.center, vec_scale(cy.axis, cy.height));
	hit_cap2 = check_cap(ray, cy, top, closest);
	if (t_body > 0 && t_body < closest)
		return (create_body_hit(ray, cy, t_body));
	if (hit_cap1.hit && hit_cap1.t < closest)
		return (hit_cap1);
	if (hit_cap2.hit && hit_cap2.t < closest)
		return (hit_cap2);
	return (create_cy_no_hit());
}
