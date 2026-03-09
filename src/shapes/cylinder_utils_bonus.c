/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:09:01 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:09:01 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_hit	create_no_hit(void)
{
	t_hit	hit;

	hit.hit = 0;
	hit.t = INFINITY;
	return (hit);
}

t_hit	create_body_hit(t_ray ray, t_cylinder cy, double t)
{
	t_hit		hit;
	t_vector	to_hit;
	double		height_proj;
	t_vector	proj_point;

	hit.hit = 1;
	hit.t = t;
	hit.point = ray_at(ray, t);
	to_hit = vec_subtract(hit.point, cy.center);
	height_proj = vec_dot(to_hit, cy.axis);
	proj_point = vec_add(cy.center, vec_scale(cy.axis, height_proj));
	hit.normal = vec_normalize(vec_subtract(hit.point, proj_point));
	hit.color = cy.color;
	hit.material = cy.material;
	hit.type = HIT_CYLINDER;
	hit.obj_center = cy.center;
	hit.obj_axis = cy.axis;
	return (hit);
}

int	check_body_hit(t_ray ray, t_cylinder cy, double t, double cl)
{
	t_vector	hit_point;
	t_vector	to_hit;
	double		h;

	if (t < EPSILON || t >= cl)
		return (0);
	hit_point = ray_at(ray, t);
	to_hit = vec_subtract(hit_point, cy.center);
	h = vec_dot(to_hit, cy.axis);
	return (h >= 0.0 && h <= cy.height);
}

static t_hit	fill_cap_hit(t_cylinder cy, t_vector pt, double t,
		t_ray ray)
{
	t_hit	hit;

	hit.hit = 1;
	hit.t = t;
	hit.point = pt;
	hit.normal = cy.axis;
	if (vec_dot(hit.normal, ray.direction) > 0)
		hit.normal = vec_negate(hit.normal);
	hit.color = cy.color;
	hit.material = cy.material;
	hit.type = HIT_CYLINDER;
	hit.obj_center = cy.center;
	hit.obj_axis = cy.axis;
	return (hit);
}

t_hit	check_cap(t_ray ray, t_cylinder cy, t_vector cap, double cl)
{
	double		denom;
	double		t;
	t_vector	diff;
	t_vector	point;

	denom = vec_dot(ray.direction, cy.axis);
	if (fabs(denom) < EPSILON)
		return (create_no_hit());
	diff = vec_subtract(cap, ray.origin);
	t = vec_dot(diff, cy.axis) / denom;
	if (t < EPSILON || t >= cl)
		return (create_no_hit());
	point = ray_at(ray, t);
	if (vec_distance(point, cap) > cy.radius)
		return (create_no_hit());
	return (fill_cap_hit(cy, point, t, ray));
}
