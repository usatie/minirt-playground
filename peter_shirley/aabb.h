#ifndef AABB_H
# define AABB_H

#include "rtweekend.h"


typedef struct s_aabb t_aabb;

struct s_aabb {
	t_point	min;
	t_point max;
};

#endif