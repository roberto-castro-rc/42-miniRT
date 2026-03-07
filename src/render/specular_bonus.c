#include "minirt_bonus.h"

t_color	calc_specular(t_hit hit, t_light light,
		t_vector view, t_vector ldir)
{
	t_vector	reflect_dir;
	double	spec;
	t_color	result;

	reflect_dir = vec_reflect(vec_negate(ldir), hit.normal);
	spec = fmax(0.0, vec_dot(reflect_dir, view));
	spec = pow(spec, SHININESS) * light.brightness;
	result = scale_color(light.color, spec);
	return (result);
}
