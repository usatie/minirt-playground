#ifndef RAY_H
# define RAY_H

# include "vec3.h"

typedef struct s_ray	t_ray;

struct s_ray {
	t_point	origin;
	t_vec3	direction;
};

// ray.c
t_ray	new_ray(t_point p, t_vec3 dir);
t_point	ray_at(const t_ray *ray, double t);
t_color	ray_color(t_ray *r);

#endif
