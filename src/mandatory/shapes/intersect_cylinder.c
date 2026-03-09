/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:09:57 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:09:58 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_hit	intersect_cylinder(t_ray ray, t_cylinder cy, double closest)
{
	t_hit		body;
	t_hit		cap1;
	t_hit		cap2;
	t_vector	top;
	double		t_body;

	t_body = check_body(ray, cy, closest);
	body = create_no_hit();
	if (t_body > 0)
	{
		body = create_body_hit(ray, cy, t_body);
		closest = t_body;
	}
	cap1 = check_cap(ray, cy, cy.center, closest);
	if (cap1.hit)
		closest = cap1.t;
	top = vec_add(cy.center, vec_scale(cy.axis, cy.height));
	cap2 = check_cap(ray, cy, top, closest);
	return (best_hit(body, best_hit(cap1, cap2)));
}
