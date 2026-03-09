#include "minirt_bonus.h"

static int	add_cone(t_scene *scene, t_cone cn)
{
	t_cone	*new;

	new = malloc(sizeof(t_cone) * (scene->cone_count + 1));
	if (!new)
		return (error_exit("Memory allocation failed"), 0);
	if (scene->cone_count > 0)
		ft_memcpy(new, scene->cones,
			sizeof(t_cone) * scene->cone_count);
	free(scene->cones);
	new[scene->cone_count++] = cn;
	scene->cones = new;
	return (1);
}

static int	validate_cone_parts(char **parts)
{
	if (!parts || !parts[1] || !parts[2]
		|| !parts[3] || !parts[4] || !parts[5])
		return (0);
	if (parts[6] && ft_strncmp(parts[6], "checker", 8))
		return (0);
	if (parts[6] && parts[7])
		return (0);
	return (1);
}

static int	parse_cone_vectors(char **parts, t_cone *cn)
{
	if (!parse_vector(parts[1], &cn->center)
		|| !parse_vector(parts[2], &cn->axis)
		|| !parse_color(parts[5], &cn->color))
		return (error_exit("Cone: invalid data"), 0);
	if (!validate_normalized(cn->axis))
		return (error_exit("Cone: axis not normalized"), 0);
	cn->axis = vec_normalize(cn->axis);
	return (1);
}

int	parse_cone(char *line, t_scene *scene)
{
	char	**parts;
	t_cone	cn;
	int		error;

	parts = ft_split(line, ' ');
	if (!validate_cone_parts(parts))
		return (free_split(parts),
			error_exit("Cone: invalid format"), 0);
	if (!parse_cone_vectors(parts, &cn))
		return (free_split(parts), 0);
	cn.half_angle = parse_double(parts[3], &error);
	cn.height = parse_double(parts[4], &error);
	if (error || cn.half_angle <= 0 || cn.half_angle >= 90
		|| cn.height <= 0)
		return (free_split(parts),
			error_exit("Cone: angle (0-90) and height > 0"), 0);
	cn.half_angle = cn.half_angle * M_PI / 180.0;
	cn.material = (parts[6] != NULL);
	free_split(parts);
	return (add_cone(scene, cn));
}
