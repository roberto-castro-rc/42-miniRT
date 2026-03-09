/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:05:16 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:17:39 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

uint32_t	color_to_int(t_color color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 0xFF);
}

void	put_pixel(mlx_image_t *img, int x, int y, t_color color)
{
	uint32_t	pixel_color;

	pixel_color = color_to_int(color);
	mlx_put_pixel(img, x, y, pixel_color);
}
