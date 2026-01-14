#include "minirt.h"

t_color	add_colors(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	return (result);
}

t_color	scale_color(t_color c, double scalar)
{
	t_color	result;

	result.r = (int)(c.r * scalar);
	result.g = (int)(c.g * scalar);
	result.b = (int)(c.b * scalar);
	return (result);
}

t_color	clamp_color(t_color c)
{
	t_color	result;

	result.r = c.r;
	result.g = c.g;
	result.b = c.b;
	if (result.r > 255)
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	if (result.r < 0)
		result.r = 0;
	if (result.g < 0)
		result.g = 0;
	if (result.b < 0)
		result.b = 0;
	return (result);
}
