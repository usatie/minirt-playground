#ifndef SPHERE_H
# define SPHERE_H

# include "hittable.h"
# include "vec3.h"

typedef t_hittable	t_sphere;

t_sphere	sphere_new(t_point cen, double r);

#endif
