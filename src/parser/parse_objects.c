/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:09:07 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 22:36:50 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	add_plane(t_scene *scene, t_plane pl)
{
	t_plane	*new;

	new = malloc(sizeof(t_plane) * (scene->plane_count + 1));
	if (!new)
		return (error_exit("Memory allocation failed"), 0);
	if (scene->plane_count > 0)
		ft_memcpy(new, scene->planes, sizeof(t_plane) * scene->plane_count);
	free(scene->planes);
	new[scene->plane_count++] = pl;
	scene->planes = new;
	return (1);
}

int	parse_plane(char *line, t_scene *scene)
{
	char	**parts;
	t_plane	pl;

	parts = ft_split(line, ' ');
	if (!parts || !parts[1] || !parts[2] || !parts[3] || parts[4])
		return (free_split(parts), error_exit("Plane: invalid format"), 0);
	if (!parse_vector(parts[1], &pl.point)
		|| !parse_vector(parts[2], &pl.normal)
		|| !parse_color(parts[3], &pl.color))
		return (free_split(parts), error_exit("Plane: invalid data"), 0);
	if (!validate_normalized(pl.normal))
		return (free_split(parts), error_exit("Plane: not normalized"), 0);
	pl.normal = vec_normalize(pl.normal);
	free_split(parts);
	return (add_plane(scene, pl));
}

static int	add_cylinder(t_scene *scene, t_cylinder cy)
{
	t_cylinder	*new;

	new = malloc(sizeof(t_cylinder) * (scene->cylinder_count + 1));
	if (!new)
		return (error_exit("Memory allocation failed"), 0);
	if (scene->cylinder_count > 0)
		ft_memcpy(new, scene->cylinders,
			sizeof(t_cylinder) * scene->cylinder_count);
	free(scene->cylinders);
	new[scene->cylinder_count++] = cy;
	scene->cylinders = new;
	return (1);
}

int	parse_cylinder(char *line, t_scene *scene)
{
	char		**parts;
	t_cylinder	cy;
	int			error;

	parts = ft_split(line, ' ');
	if (!parts || !parts[1] || !parts[2] || !parts[3]
		|| !parts[4] || !parts[5] || parts[6])
		return (free_split(parts), error_exit("Cylinder: invalid format"), 0);
	if (!parse_vector(parts[1], &cy.center)
		|| !parse_vector(parts[2], &cy.axis)
		|| !parse_color(parts[5], &cy.color))
		return (free_split(parts), error_exit("Cylinder: invalid data"), 0);
	if (!validate_normalized(cy.axis))
		return (free_split(parts),
			error_exit("Cylinder: axis not normalized"), 0);
	cy.axis = vec_normalize(cy.axis);
	cy.diameter = parse_double(parts[3], &error);
	if (error || cy.diameter <= 0 || cy.height <= 0)
		return (free_split(parts),
			error_exit("Cylinder dimensions must be > 0"), 0);
	cy.height = parse_double(parts[4], &error);
	if (error || cy.diameter <= 0 || cy.height <= 0)
		return (free_split(parts),
			error_exit("Cylinder dimensions must be > 0"), 0);
	cy.radius = cy.diameter / 2.0;
	free_split(parts);
	return (add_cylinder(scene, cy));
}
