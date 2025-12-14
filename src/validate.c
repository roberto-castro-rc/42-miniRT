#include "../includes/minirt.h"

int	validate_range_double(double val, double min, double max)
{
	return (val >= min && val <= max);
}

int	validate_range_int(int val, int min, int max)
{
	return (val >= min && val <= max);
}

int	validate_normalized(t_vector vec)
{
	double	magnitude;

	magnitude = vec_magnitude(vec);
	return (fabs(magnitude - 1.0) < 0.01);
}
