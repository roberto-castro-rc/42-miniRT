/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:05:38 by rpaulo-c          #+#    #+#             */
/*   Updated: 2026/03/09 18:29:03 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

static int	parse_elem(char *id, char *line, t_scene *scene)
{
	if (ft_strncmp(id, "L", 2) == 0)
		return (parse_light(line, scene));
	if (ft_strncmp(id, "sp", 3) == 0)
		return (parse_sphere(line, scene));
	if (ft_strncmp(id, "pl", 3) == 0)
		return (parse_plane(line, scene));
	if (ft_strncmp(id, "cy", 3) == 0)
		return (parse_cylinder(line, scene));
	if (ft_strncmp(id, "cn", 3) == 0)
		return (parse_cone(line, scene));
	if (id[0] != '\n' && id[0] != '\0')
		return (error_exit("Unknown element type"), 0);
	return (1);
}

static int	parse_line(char *line, t_scene *scene, int *flags)
{
	char	**parts;
	int		result;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0])
		return (free_split(parts), 1);
	if (ft_strncmp(parts[0], "A", 2) == 0 && !(*flags & 1))
	{
		result = parse_ambient(parts, scene);
		if (result)
			*flags |= 1;
	}
	else if (ft_strncmp(parts[0], "C", 2) == 0 && !(*flags & 2))
	{
		result = parse_camera(parts, scene);
		if (result)
			*flags |= 2;
	}
	else
		result = parse_elem(parts[0], line, scene);
	free_split(parts);
	return (result);
}

static char	*trim_line(char *line)
{
	char	*trimmed;
	int		len;

	trimmed = ft_strtrim(line, " \t\n\r");
	if (!trimmed)
		return (NULL);
	len = ft_strlen(trimmed);
	if (len == 0 || trimmed[0] == '#')
	{
		free(trimmed);
		return (NULL);
	}
	return (trimmed);
}

static void	drain_gnl(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}

int	parse_scene(char *filename, t_scene *scene)
{
	int		fd;
	char	*line;
	char	*trimmed;
	int		flags;

	flags = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (error_exit("Failed to open scene file"), 0);
	line = get_next_line(fd);
	while (line)
	{
		trimmed = trim_line(line);
		free(line);
		if (trimmed && !parse_line(trimmed, scene, &flags))
			return (free(trimmed), drain_gnl(fd), close(fd), 0);
		if (trimmed)
			free(trimmed);
		line = get_next_line(fd);
	}
	close(fd);
	if ((flags & 3) != 3)
		return (error_exit("Missing Ambient or Camera definition"), 0);
	return (1);
}
