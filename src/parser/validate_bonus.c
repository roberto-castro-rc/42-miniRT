#include "minirt_bonus.h"

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
	if (vec.x < -1.0 || vec.x > 1.0)
		return (0);
	if (vec.y < -1.0 || vec.y > 1.0)
		return (0);
	if (vec.z < -1.0 || vec.z > 1.0)
		return (0);
	return (vec_magnitude(vec) > EPSILON);
}

int	validate_int_str(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
