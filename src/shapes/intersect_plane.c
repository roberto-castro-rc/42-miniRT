/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_plane.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:10:02 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:10:03 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_hit	intersect_plane(t_ray ray, t_plane pl, double closest)
{
	double		denom;
	double		t;
	t_vector	diff;
	t_hit		hit;

	denom = vec_dot(ray.direction, pl.normal);
	if (fabs(denom) < EPSILON)
		return (create_no_hit());
	diff = vec_subtract(pl.point, ray.origin);
	t = vec_dot(diff, pl.normal) / denom;
	if (t < EPSILON || t >= closest)
		return (create_no_hit());
	hit.hit = 1;
	hit.t = t;
	hit.point = ray_at(ray, t);
	hit.normal = pl.normal;
	if (vec_dot(hit.normal, ray.direction) > 0)
		hit.normal = vec_negate(hit.normal);
	hit.color = pl.color;
	return (hit);
}
