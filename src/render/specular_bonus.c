#include "minirt_bonus.h"

/*
** calc_specular - stub for Step 2 (Phong specular bonus)
** Will compute specular highlight:
**   R = reflect(-L, N)
**   spec = pow(max(dot(R, V), 0), SHININESS) * brightness
*/
t_color	calc_specular(t_hit hit, t_light light,
		t_vector view, t_vector ldir)
{
	(void)hit;
	(void)light;
	(void)view;
	(void)ldir;
	return ((t_color){0, 0, 0});
}
