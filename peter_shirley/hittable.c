#include "hittable.h"
#include "sphere.h"

bool	hit_single(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	if (self->type == SPHERE)
		return (sphere_hit_single(self, r, t_min, t_max, rec));
	else if (self->type == XY_RECT)
		return (xyrect_hit_single(self, r, t_min, t_max, rec));
	else
		return (false);
}

bool	sphere_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	t_vec3	a = sub_vec3(self->center, new_vec3(self->radius, self->radius, self->radius));
	t_vec3	b = add_vec3(self->center, new_vec3(self->radius, self->radius, self->radius));
	*output_box = new_aabb(&a, &b);
	return (true);
}

bool	bounding_box(const t_hittable *self, t_aabb *output_box)
{
	if (self->type == SPHERE)
		return (sphere_bounding_box(self, output_box));
	else
		return (false);
}
