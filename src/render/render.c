/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:09:29 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:09:30 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
