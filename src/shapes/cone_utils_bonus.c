/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:09:55 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:09:55 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_hit	create_cone_body_hit(t_ray ray, t_cone cn, double t)
{
	t_hit		hit;
	t_vector	cp;
	double		h;
	t_vector	proj;

	hit.hit = 1;
	hit.t = t;
	hit.point = ray_at(ray, t);
	cp = vec_subtract(hit.point, cn.center);
	h = vec_dot(cp, cn.axis);
	proj = vec_add(cn.center, vec_scale(cn.axis, h));
	hit.normal = vec_subtract(hit.point, proj);
	hit.normal = vec_normalize(hit.normal);
	hit.normal = vec_subtract(vec_scale(hit.normal, cos(cn.half_angle)),
			vec_scale(cn.axis, sin(cn.half_angle)));
	hit.normal = vec_normalize(hit.normal);
	if (vec_dot(hit.normal, ray.direction) > 0)
		hit.normal = vec_negate(hit.normal);
	hit.color = cn.color;
	hit.material = cn.material;
	hit.type = HIT_CONE;
	hit.obj_center = cn.center;
	hit.obj_axis = cn.axis;
	return (hit);
}

int	check_cone_body(t_ray ray, t_cone cn, double t, double cl)
{
	t_vector	hit_point;
	t_vector	cp;
	double		h;

	if (t < EPSILON || t >= cl)
		return (0);
	hit_point = ray_at(ray, t);
	cp = vec_subtract(hit_point, cn.center);
	h = vec_dot(cp, cn.axis);
	return (h >= 0.0 && h <= cn.height);
}

t_hit	check_cone_cap(t_ray ray, t_cone cn, t_vector cap, double cl)
{
	double		denom;
	double		t;
	t_hit		hit;

	denom = vec_dot(ray.direction, cn.axis);
	if (fabs(denom) < EPSILON)
		return (create_no_hit());
	t = vec_dot(vec_subtract(cap, ray.origin), cn.axis) / denom;
	if (t < EPSILON || t >= cl)
		return (create_no_hit());
	hit.point = ray_at(ray, t);
	if (vec_distance(hit.point, cap) > cn.height * tan(cn.half_angle))
		return (create_no_hit());
	hit.hit = 1;
	hit.t = t;
	hit.normal = cn.axis;
	if (vec_dot(hit.normal, ray.direction) > 0)
		hit.normal = vec_negate(hit.normal);
	hit.color = cn.color;
	hit.material = cn.material;
	hit.type = HIT_CONE;
	hit.obj_center = cn.center;
	hit.obj_axis = cn.axis;
	return (hit);
}
