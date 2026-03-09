/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 00:00:00 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:16:28 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

static t_vector	calc_perturbation(t_vector p, double s)
{
	t_vector	noise;

	noise.x = sin(p.x * s) * cos(p.z * s);
	noise.y = sin(p.y * s) * cos(p.x * s);
	noise.z = sin(p.z * s) * cos(p.y * s);
	return (noise);
}

void	apply_bump(t_hit *hit)
{
	t_vector	perturb;
	t_vector	bumped;

	perturb = calc_perturbation(hit->point, BUMP_SCALE);
	perturb = vec_scale(perturb, BUMP_INTENSITY);
	bumped = vec_add(hit->normal, perturb);
	hit->normal = vec_normalize(bumped);
}
