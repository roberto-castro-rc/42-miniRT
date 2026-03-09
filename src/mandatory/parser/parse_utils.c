/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:09:16 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:09:16 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_split(char **parts)
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

double	parse_double(char *str, int *error)
{
	int		end;
	double	result;

	*error = 0;
	result = ft_atod(str, &end);
	if (str[end] != '\0' && str[end] != ',' && str[end] != ' ')
		*error = 1;
	return (result);
}

static int	parse_component(char *str, double *val)
{
	int		error;

	*val = parse_double(str, &error);
	return (!error);
}

int	parse_vector(char *str, t_vector *vec)
{
	char	**parts;

	parts = ft_split(str, ',');
	if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
	{
		if (parts)
			free_split(parts);
		return (0);
	}
	if (!parse_component(parts[0], &vec->x)
		|| !parse_component(parts[1], &vec->y)
		|| !parse_component(parts[2], &vec->z))
	{
		free_split(parts);
		return (0);
	}
	free_split(parts);
	return (1);
}

int	parse_color(char *str, t_color *color)
{
	char	**parts;
	int		r;
	int		g;
	int		b;

	parts = ft_split(str, ',');
	if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
		return (free_split(parts), 0);
	if (!validate_int_str(parts[0]) || !validate_int_str(parts[1])
		|| !validate_int_str(parts[2]))
		return (free_split(parts), 0);
	r = ft_atoi(parts[0]);
	g = ft_atoi(parts[1]);
	b = ft_atoi(parts[2]);
	free_split(parts);
	if (!validate_range_int(r, 0, 255)
		|| !validate_range_int(g, 0, 255)
		|| !validate_range_int(b, 0, 255))
		return (0);
	color->r = r;
	color->g = g;
	color->b = b;
	return (1);
}
