/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 17:33:17 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 19:33:09 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	main(int argc, char **argv)
{
	t_scene	scene;

	if (argc != 2)
	{
		error_exit("Usage: ./miniRT <scene.rt>");
		return (1);
	}
	if (!check_file_extension(argv[1], ".rt"))
	{
		error_exit("Scene file must have .rt extension");
		return (1);
	}
	init_scene(&scene);
	if (!parse_scene(argv[1], &scene))
	{
		cleanup_scene(&scene);
		return (1);
	}
	init_mlx(&scene, argv[1]);
	init_camera(&scene.camera);
	render_scene(&scene);
	mlx_loop(scene.mlx);
	cleanup_scene(&scene);
	return (0);
}
