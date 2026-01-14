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

static int	add_sphere(t_scene *scene, t_sphere sp)
{
	t_sphere	*new;

	new = malloc(sizeof(t_sphere) * (scene->sphere_count + 1));
	if (!new)
		return (error_exit("Memory allocation failed"), 0);
	if (scene->sphere_count > 0)
		ft_memcpy(new, scene->spheres,
			sizeof(t_sphere) * scene->sphere_count);
	free(scene->spheres);
	new[scene->sphere_count++] = sp;
	scene->spheres = new;
	return (1);
}

int	parse_sphere(char *line, t_scene *scene)
{
	char		**parts;
	t_sphere	sp;
	int			error;

	parts = ft_split(line, ' ');
	if (!parts || !parts[1] || !parts[2] || !parts[3])
		return (error_exit("Sphere: invalid format"), 0);
	if (!parse_vector(parts[1], &sp.center)
		|| !parse_color(parts[3], &sp.color))
		return (free_split(parts), error_exit("Sphere: invalid data"), 0);
	sp.diameter = parse_double(parts[2], &error);
	if (error || sp.diameter <= 0)
		return (free_split(parts),
			error_exit("Sphere diameter must be > 0"), 0);
	sp.radius = sp.diameter / 2.0;
	free_split(parts);
	return (add_sphere(scene, sp));
}
