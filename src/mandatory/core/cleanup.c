/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:08:27 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/09 18:17:50 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	cleanup_scene(t_scene *scene)
{
	if (scene->spheres)
		free(scene->spheres);
	if (scene->planes)
		free(scene->planes);
	if (scene->cylinders)
		free(scene->cylinders);
	if (scene->lights)
		free(scene->lights);
	if (scene->image)
		mlx_delete_image(scene->mlx, scene->image);
	if (scene->mlx)
		mlx_terminate(scene->mlx);
}
