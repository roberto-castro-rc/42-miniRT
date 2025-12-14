#ifndef VECTORS_H
# define VECTORS_H

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}	t_ray;

/* Basic vector operations */
t_vector	vec_create(double x, double y, double z);
t_vector	vec_add(t_vector a, t_vector b);
t_vector	vec_subtract(t_vector a, t_vector b);
t_vector	vec_scale(t_vector v, double scalar);
t_vector	vec_negate(t_vector v);

/* Vector products */
double		vec_dot(t_vector a, t_vector b);
t_vector	vec_cross(t_vector a, t_vector b);

/* Vector utilities */
double		vec_magnitude(t_vector v);
t_vector	vec_normalize(t_vector v);
double		vec_distance(t_vector a, t_vector b);

/* Ray utilities */
t_ray		ray_create(t_vector origin, t_vector direction);
t_vector	ray_at(t_ray ray, double t);

#endif
