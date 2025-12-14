#include "../includes/minirt.h"

static t_color	calc_ambient(t_color obj_color, t_ambient ambient)
{
	t_color	result;

	result.r = (int)(obj_color.r * ambient.ratio);
	result.g = (int)(obj_color.g * ambient.ratio);
	result.b = (int)(obj_color.b * ambient.ratio);
	return (result);
}

static t_color	calc_diffuse(t_color obj, t_light light, t_vector n, t_vector l)
{
	double	diffuse_factor;
	t_color	result;

	diffuse_factor = fmax(0.0, vec_dot(n, l));
	diffuse_factor *= light.brightness;
	result.r = (int)(obj.r * diffuse_factor);
	result.g = (int)(obj.g * diffuse_factor);
	result.b = (int)(obj.b * diffuse_factor);
	return (result);
}

static t_color	apply_lights(t_scene *scene, t_hit hit)
{
	t_color		total;
	t_color		diffuse;
	t_vector	light_dir;
	t_vector	offset_point;
	int			i;

	total = (t_color){0, 0, 0};
	offset_point = vec_add(hit.point, vec_scale(hit.normal, EPSILON));
	i = 0;
	while (i < scene->light_count)
	{
		if (!check_shadow(scene, offset_point, scene->lights[i]))
		{
			light_dir = vec_normalize(
					vec_subtract(scene->lights[i].position, hit.point));
			diffuse = calc_diffuse(hit.color, scene->lights[i],
					hit.normal, light_dir);
			total = add_colors(total, diffuse);
		}
		i++;
	}
	return (total);
}

t_color	calculate_lighting(t_scene *scene, t_hit hit, t_ray ray)
{
	t_color	ambient;
	t_color	diffuse;
	t_color	final;

	(void)ray;
	ambient = calc_ambient(hit.color, scene->ambient);
	diffuse = apply_lights(scene, hit);
	final = add_colors(ambient, diffuse);
	final = clamp_color(final);
	return (final);
}
