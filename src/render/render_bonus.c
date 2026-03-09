/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:10:16 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:10:16 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	render_scene(t_scene *scene)
{
	int		x;
	int		y;
	t_color	color;

	y = 0;
	while (y < scene->height)
	{
		x = 0;
		while (x < scene->width)
		{
			color = trace_ray(scene, x, y);
			put_pixel(scene->image, x, y, color);
			x++;
		}
		y++;
	}
}
