#ifndef MATERIAL_H
# define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

typedef struct s_material	t_material;
typedef t_material	t_lambertian;

struct s_material {
	// lambertian
	t_color	albedo;
};

bool	scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered);

#endif
