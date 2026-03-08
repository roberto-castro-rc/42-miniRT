/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:19:42 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 20:34:04 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_ambient_flag(char **parts, t_scene *scene, int *flags)
{
	int	result;

	result = parse_ambient(parts, scene);
	if (result)
		*flags |= 1;
	return (result);
}

int	parse_camera_flag(char **parts, t_scene *scene, int *flags)
{
	int	result;

	result = parse_camera(parts, scene);
	if (result)
		*flags |= 2;
	return (result);
}

int	parse_light_flag(char *line, t_scene *scene, int *flags)
{
	*flags |= 4;
	return (parse_light(line, scene));
}
