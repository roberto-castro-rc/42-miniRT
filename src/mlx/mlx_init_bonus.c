/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:05:12 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:17:40 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	init_mlx(t_scene *scene)
{
	scene->mlx = mlx_init(scene->width, scene->height, "miniRT", true);
	if (!scene->mlx)
	{
		error_exit("Failed to initialize MLX42");
		exit(1);
	}
	scene->image = mlx_new_image(scene->mlx, scene->width, scene->height);
	if (!scene->image)
	{
		mlx_terminate(scene->mlx);
		error_exit("Failed to create image");
		exit(1);
	}
	if (mlx_image_to_window(scene->mlx, scene->image, 0, 0) < 0)
	{
		mlx_delete_image(scene->mlx, scene->image);
		mlx_terminate(scene->mlx);
		error_exit("Failed to put image to window");
		exit(1);
	}
	mlx_key_hook(scene->mlx, key_hook, scene);
	mlx_close_hook(scene->mlx, close_hook, scene);
}
