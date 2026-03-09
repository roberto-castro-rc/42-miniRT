/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:05:12 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 19:33:09 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

static char	*get_title(char *file)
{
	char	*name;
	char	*title;

	name = ft_strrchr(file, '/');
	if (name)
		name++;
	else
		name = file;
	title = ft_strjoin("miniRT-Bonus | ", name);
	return (title);
}

static void	init_image(t_scene *scene)
{
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
}

void	init_mlx(t_scene *scene, char *file)
{
	char	*title;

	title = get_title(file);
	scene->mlx = mlx_init(scene->width, scene->height, title, true);
	free(title);
	if (!scene->mlx)
	{
		error_exit("Failed to initialize MLX42");
		exit(1);
	}
	init_image(scene);
	mlx_key_hook(scene->mlx, key_hook, scene);
	mlx_close_hook(scene->mlx, close_hook, scene);
}
