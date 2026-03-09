#include "vectors_bonus.h"

t_vector	vec_reflect(t_vector incident, t_vector normal)
{
	double		dot;
	t_vector	result;

	dot = vec_dot(incident, normal);
	result = vec_subtract(incident, vec_scale(normal, 2.0 * dot));
	return (result);
}
