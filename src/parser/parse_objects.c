#include "minirt.h"

static void	free_split(char **parts)
{
	int	i;

	if (!parts)
		return ;
	i = 0;
	while (parts[i])
	{
		free(parts[i]);
		i++;
	}
	free(parts);
}

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
	if (!parts || !parts[1] || !parts[2] || !parts[3])
		return (error_exit("Plane: invalid format"), 0);
	if (!parse_vector(parts[1], &pl.point)
		|| !parse_vector(parts[2], &pl.normal)
		|| !parse_color(parts[3], &pl.color))
		return (free_split(parts), error_exit("Plane: invalid data"), 0);
	if (!validate_normalized(pl.normal))
		return (free_split(parts),
			error_exit("Plane normal must be normalized"), 0);
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
		|| !parts[4] || !parts[5])
		return (error_exit("Cylinder: invalid format"), 0);
	if (!parse_vector(parts[1], &cy.center)
		|| !parse_vector(parts[2], &cy.axis)
		|| !parse_color(parts[5], &cy.color))
		return (free_split(parts), error_exit("Cylinder: invalid data"), 0);
	if (!validate_normalized(cy.axis))
		return (free_split(parts),
			error_exit("Cylinder axis must be normalized"), 0);
	cy.diameter = parse_double(parts[3], &error);
	cy.height = parse_double(parts[4], &error);
	if (error || cy.diameter <= 0 || cy.height <= 0)
		return (free_split(parts),
			error_exit("Cylinder dimensions must be > 0"), 0);
	cy.radius = cy.diameter / 2.0;
	free_split(parts);
	return (add_cylinder(scene, cy));
}
