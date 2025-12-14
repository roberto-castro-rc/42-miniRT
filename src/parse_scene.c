#include "../includes/minirt.h"

static void	free_split(char **parts)
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

static int	parse_line(char *line, t_scene *scene, int *flags)
{
	char	**parts;
	int		result;

	parts = ft_split(line, ' ');
	if (!parts || !parts[0])
		return (free_split(parts), 1);
	result = 1;
	if (ft_strncmp(parts[0], "A", 2) == 0 && (*flags & 1) == 0)
		result = (parse_ambient(parts, scene), *flags |= 1, 1);
	else if (ft_strncmp(parts[0], "C", 2) == 0 && (*flags & 2) == 0)
		result = (parse_camera(parts, scene), *flags |= 2, 1);
	else if (ft_strncmp(parts[0], "L", 2) == 0)
		result = parse_light(line, scene);
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
			return (free(trimmed), close(fd), 0);
		if (trimmed)
			free(trimmed);
		line = get_next_line(fd);
	}
	close(fd);
	if ((flags & 3) != 3)
		return (error_exit("Missing Ambient or Camera definition"), 0);
	return (1);
}
