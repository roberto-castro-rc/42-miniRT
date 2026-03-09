#include "minirt_bonus.h"

static double	parse_fraction(char *str, int *i)
{
	double	frac;
	double	div;

	frac = 0.0;
	div = 10.0;
	while (ft_isdigit(str[*i]))
	{
		frac += (str[*i] - '0') / div;
		div *= 10.0;
		(*i)++;
	}
	return (frac);
}

double	ft_atod(char *str, int *end)
{
	int		i;
	int		sign;
	double	result;

	i = 0;
	sign = 1;
	result = 0.0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = result * 10.0 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.')
	{
		i++;
		result += parse_fraction(str, &i);
	}
	*end = i;
	return (sign * result);
}
