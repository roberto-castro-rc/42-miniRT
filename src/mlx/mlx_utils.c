/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:08:57 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:08:57 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
