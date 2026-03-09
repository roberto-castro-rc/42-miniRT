/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_reflect_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:04:36 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:17:46 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors_bonus.h"

t_vector	vec_reflect(t_vector incident, t_vector normal)
{
	double		dot;
	t_vector	result;

	dot = vec_dot(incident, normal);
	result = vec_subtract(incident, vec_scale(normal, 2.0 * dot));
	return (result);
}
