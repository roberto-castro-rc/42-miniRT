#ifndef MINIRT_H
# define MINIRT_H

# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include "../lib/libft/libft.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "vectors.h"

# define WIDTH 1280
# define HEIGHT 720
# define EPSILON 1e-6

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_ambient
{
	double	ratio;
	t_color	color;
}	t_ambient;

typedef struct s_camera
{
	t_vector	position;
	t_vector	orientation;
	double		fov;
	t_vector	right;
	t_vector	up;
	double		aspect_ratio;
	double		viewport_height;
	double		viewport_width;
}	t_camera;

typedef struct s_light
{
	t_vector	position;
	double		brightness;
	t_color		color;
}	t_light;

typedef struct s_sphere
{
	t_vector	center;
	double		diameter;
	double		radius;
	t_color		color;
}	t_sphere;

typedef struct s_plane
{
	t_vector	point;
	t_vector	normal;
	t_color		color;
}	t_plane;

typedef struct s_cylinder
{
	t_vector	center;
	t_vector	axis;
	double		diameter;
	double		radius;
	double		height;
	t_color		color;
}	t_cylinder;

typedef struct s_hit
{
	int			hit;
	double		t;
	t_vector	point;
	t_vector	normal;
	t_color		color;
}	t_hit;

typedef struct s_scene
{
	t_ambient		ambient;
	t_camera		camera;
	t_light			*lights;
	int				light_count;
	t_sphere		*spheres;
	int				sphere_count;
	t_plane			*planes;
	int				plane_count;
	t_cylinder		*cylinders;
	int				cylinder_count;
	mlx_t			*mlx;
	mlx_image_t		*image;
	int				width;
	int				height;
}	t_scene;

/* Parsing functions */
int			parse_scene(char *filename, t_scene *scene);
int			parse_ambient(char **parts, t_scene *scene);
int			parse_camera(char **parts, t_scene *scene);
int			parse_light(char *line, t_scene *scene);
int			parse_sphere(char *line, t_scene *scene);
int			parse_plane(char *line, t_scene *scene);
int			parse_cylinder(char *line, t_scene *scene);

/* Parsing utilities */
int			parse_vector(char *str, t_vector *vec);
int			parse_color(char *str, t_color *color);
double		parse_double(char *str, int *error);
int			validate_normalized(t_vector vec);
int			validate_range_double(double val, double min, double max);
int			validate_range_int(int val, int min, int max);

/* Initialization and cleanup */
void		init_scene(t_scene *scene);
void		init_mlx(t_scene *scene);
void		init_camera(t_camera *cam);
void		cleanup_scene(t_scene *scene);

/* Rendering functions */
void		render_scene(t_scene *scene);
t_color		trace_ray(t_scene *scene, int x, int y);
t_ray		generate_ray(t_scene *scene, int x, int y);
t_hit		find_closest_hit(t_scene *scene, t_ray ray);

/* Intersection functions */
t_hit		intersect_sphere(t_ray ray, t_sphere sp, double closest);
t_hit		intersect_plane(t_ray ray, t_plane pl, double closest);
t_hit		intersect_cylinder(t_ray ray, t_cylinder cy, double closest);

/* Cylinder utilities */
t_hit		create_cy_no_hit(void);
t_hit		create_body_hit(t_ray ray, t_cylinder cy, double t);
int			check_body_hit(t_ray ray, t_cylinder cy, double t, double cl);
t_hit		check_cap(t_ray ray, t_cylinder cy, t_vector cap, double cl);

/* Lighting functions */
t_color		calculate_lighting(t_scene *scene, t_hit hit, t_ray ray);
int			check_shadow(t_scene *scene, t_vector point, t_light light);
t_color		add_colors(t_color c1, t_color c2);
t_color		scale_color(t_color c, double scalar);
t_color		clamp_color(t_color c);

/* MLX utilities */
uint32_t	color_to_int(t_color color);
void		put_pixel(mlx_image_t *img, int x, int y, t_color color);
void		key_hook(mlx_key_data_t keydata, void *param);
void		close_hook(void *param);

/* Error handling */
void		error_exit(char *message);
int			check_file_extension(char *filename, char *ext);

#endif
