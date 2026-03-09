/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 00:00:00 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 13:44:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

static int	checker_pattern(double u, double v)
{
	int	iu;
	int	iv;

	iu = (int)floor(u * CHECKER_SCALE);
	iv = (int)floor(v * CHECKER_SCALE);
	return ((iu + iv) % 2 != 0);
}

static t_vector	get_tangent(t_vector axis)
{
	t_vector	ref;

	ref = vec_create(0, 1, 0);
	if (fabs(axis.y) > 0.9)
		ref = vec_create(1, 0, 0);
	return (vec_normalize(vec_cross(axis, ref)));
}

static void	checker_sphere(t_hit *hit)
{
	double	u;
	double	v;

	u = 0.5 + atan2(hit->normal.z, hit->normal.x) / (2.0 * M_PI);
	v = 0.5 - asin(hit->normal.y) / M_PI;
	if (checker_pattern(u, v))
		hit->color = (t_color){0, 0, 0};
}

static void	checker_plane(t_hit *hit)
{
	t_vector	tan;
	t_vector	bitan;
	double		u;
	double		v;

	tan = get_tangent(hit->normal);
	bitan = vec_cross(hit->normal, tan);
	u = vec_dot(hit->point, tan);
	v = vec_dot(hit->point, bitan);
	if (checker_pattern(u, v))
		hit->color = (t_color){0, 0, 0};
}

void	apply_checkerboard(t_hit *hit)
{
	t_vector	cp;
	t_vector	tan;
	double		uv[2];

	if (hit->type == HIT_SPHERE)
		return (checker_sphere(hit));
	if (hit->type == HIT_PLANE)
		return (checker_plane(hit));
	cp = vec_subtract(hit->point, hit->obj_center);
	tan = get_tangent(hit->obj_axis);
	uv[0] = atan2(vec_dot(cp, vec_cross(hit->obj_axis, tan)),
			vec_dot(cp, tan)) / (2.0 * M_PI) + 0.5;
	uv[1] = vec_dot(cp, hit->obj_axis);
	if (checker_pattern(uv[0], uv[1]))
		hit->color = (t_color){0, 0, 0};
}
