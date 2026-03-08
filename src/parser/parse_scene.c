/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaula-n <lpaula-n@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:09:10 by lpaula-n          #+#    #+#             */
/*   Updated: 2026/03/07 17:30:40 by lpaula-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	parse_line(char *line, t_scene *scene, int *flags)
{
	char	**parts;
	int		result;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0])
		return (free_split(parts), 1);
	result = 1;
	if (ft_strncmp(parts[0], "A", 2) == 0 && !(*flags & 1))
		result = parse_ambient_flag(parts, scene, flags);
	else if (ft_strncmp(parts[0], "C", 2) == 0 && !(*flags & 2))
		result = parse_camera_flag(parts, scene, flags);
	else if (ft_strncmp(parts[0], "L", 2) == 0 && !(*flags & 4))
		result = parse_light_flag(line, scene, flags);
	else if (ft_strncmp(parts[0], "sp", 3) == 0)
		result = parse_sphere(line, scene);
	else if (ft_strncmp(parts[0], "pl", 3) == 0)
		result = parse_plane(line, scene);
	else if (ft_strncmp(parts[0], "cy", 3) == 0)
		result = parse_cylinder(line, scene);
	else if (parts[0][0] != '\n' && parts[0][0] != '\0')
		return (free_split(parts), error_exit("Unknown element type"), 0);
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
