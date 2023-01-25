#ifndef SPHERE_H
# define SPHERE_H

# include "hittable.h"
# include "vec3.h"

typedef t_hittable	t_sphere;

t_sphere	sphere_new(t_point cen, double r, t_material *m);
t_sphere	*sphere_alloc(t_point cen, double r, t_material *m);
bool		sphere_hit_single(const t_sphere *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec);

#endif
