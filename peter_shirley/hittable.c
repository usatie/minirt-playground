#include "hittable.h"
#include "sphere.h"

bool	hit_single(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	// if SPHERE
	if (self->type == SPHERE)
		return (sphere_hit_single(self, r, t_min, t_max, rec));
	else if (self->type == XY_RECT)
		return (xyrect_hit_single(self, r, t_min, t_max, rec));
	else
		return (false);
}
