#ifndef HITTABLE_H
# define HITTABLE_H

# include <stdbool.h>
# include "ray.h"

typedef struct s_hit_record	t_hit_record;
typedef struct s_hittable	t_hittable;

struct s_hit_record {
	t_point	p;
	t_vec3	normal;
	double	t;
};

struct s_hittable {
	// SPHERE
	t_point	center;
	double	radius;
};

bool	hit(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec);

#endif
