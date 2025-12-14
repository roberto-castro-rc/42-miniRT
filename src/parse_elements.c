#include "../includes/minirt.h"

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

int	parse_ambient(char **parts, t_scene *scene)
{
	int		error;
	double	ratio;

	if (!parts[1] || !parts[2])
		return (error_exit("Ambient: invalid format"), 0);
	ratio = parse_double(parts[1], &error);
	if (error || !validate_range_double(ratio, 0.0, 1.0))
		return (error_exit("Ambient ratio must be in [0.0, 1.0]"), 0);
	if (!parse_color(parts[2], &scene->ambient.color))
		return (error_exit("Ambient: invalid color format"), 0);
	scene->ambient.ratio = ratio;
	return (1);
}

int	parse_camera(char **parts, t_scene *scene)
{
	int		error;
	double	fov;

	if (!parts[1] || !parts[2] || !parts[3])
		return (error_exit("Camera: invalid format"), 0);
	if (!parse_vector(parts[1], &scene->camera.position))
		return (error_exit("Camera: invalid position"), 0);
	if (!parse_vector(parts[2], &scene->camera.orientation))
		return (error_exit("Camera: invalid orientation"), 0);
	if (!validate_normalized(scene->camera.orientation))
		return (error_exit("Camera: orientation must be normalized"), 0);
	fov = parse_double(parts[3], &error);
	if (error || !validate_range_double(fov, 0.0, 180.0))
		return (error_exit("Camera FOV must be in [0, 180]"), 0);
	scene->camera.fov = fov;
	return (1);
}

int	parse_light(char *line, t_scene *scene)
{
	char		**parts;
	t_light		*new_lights;
	t_light		light;
	int			error;

	parts = ft_split(line, ' ');
	if (!parts || !parts[1] || !parts[2] || !parts[3])
		return (error_exit("Light: invalid format"), 0);
	if (!parse_vector(parts[1], &light.position)
		|| !parse_color(parts[3], &light.color))
		return (free_split(parts), error_exit("Light: invalid data"), 0);
	light.brightness = parse_double(parts[2], &error);
	if (error || !validate_range_double(light.brightness, 0.0, 1.0))
		return (free_split(parts), error_exit("Light brightness [0, 1]"), 0);
	new_lights = malloc(sizeof(t_light) * (scene->light_count + 1));
	if (!new_lights)
		return (free_split(parts), error_exit("Memory allocation failed"), 0);
	if (scene->light_count > 0)
		ft_memcpy(new_lights, scene->lights,
			sizeof(t_light) * scene->light_count);
	free(scene->lights);
	new_lights[scene->light_count++] = light;
	scene->lights = new_lights;
	return (free_split(parts), 1);
}
