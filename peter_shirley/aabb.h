#ifndef AABB_H
# define AABB_H

#include "rtweekend.h"


typedef struct s_aabb t_aabb;

struct s_aabb {
	t_point	min;
	t_point max;
};

t_aabb	new_aabb(t_point *a, t_point *b);
t_aabb	*alloc_aabb(t_point *a, t_point *b);
bool	hit_aabb(const t_aabb *self, const t_ray *r, double tmin, double tmax);
t_aabb	surrounding_box(t_aabb box0, t_aabb box1);
#endif
