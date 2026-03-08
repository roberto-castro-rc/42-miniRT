/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:09:34 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:09:34 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_shadow(t_scene *scene, t_vector point, t_light light)
{
	t_ray		shadow_ray;
	t_vector	to_light;
	double		light_distance;
	t_hit		hit;

	to_light = vec_subtract(light.position, point);
	light_distance = vec_magnitude(to_light);
	shadow_ray.origin = point;
	shadow_ray.direction = vec_normalize(to_light);
	hit = find_closest_hit(scene, shadow_ray);
	if (hit.hit && hit.t < light_distance)
		return (1);
	return (0);
}
