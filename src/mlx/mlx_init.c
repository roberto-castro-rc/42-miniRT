/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:08:54 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:08:55 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
