/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:08:27 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:08:28 by lpaula-n         ###   ########.fr       */
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
