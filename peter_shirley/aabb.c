#include <stdlib.h>
#include "aabb.h"

t_aabb	new_aabb(t_point *a, t_point *b)
{
	t_aabb	self;

	self.min = *a;
	self.max = *b;
	return (self);
}

t_aabb	*alloc_aabb(t_point *a, t_point *b)
{
	t_aabb	*self;

	self = calloc(1, sizeof(*self));
	*self = new_aabb(a, b);
	return (self); 
}


static void	swap(double *a, double *b)
{
	double tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

bool	hit_aabb(const t_aabb *self, const t_ray *r, double tmin, double tmax)
{
	{
		float	invD = 1.0f / r->direction.x; 
		double t0 = (self->min.x - r->origin.x) * invD;
		double t1 = (self->max.x - r->origin.x) * invD;
		if (invD < 0.0f)
			swap(&t0, &t1);
		tmin = fmax(t0, tmin);
		tmax = fmin(t1, tmax);
		if (tmax <= tmin)
			return (false);
	}
	{
		float	invD = 1.0f / r->direction.y; 
		double t0 = (self->min.y - r->origin.y) * invD;
		double t1 = (self->max.y - r->origin.y) * invD;
		if (invD < 0.0f)
			swap(&t0, &t1);
		tmin = fmax(t0, tmin);
		tmax = fmin(t1, tmax);
		if (tmax <= tmin)
			return (false);
	}
	{
		float	invD = 1.0f / r->direction.z; 
		double t0 = (self->min.z - r->origin.z) * invD;
		double t1 = (self->max.z - r->origin.z) * invD;
		if (invD < 0.0f)
			swap(&t0, &t1);
		tmin = fmax(t0, tmin);
		tmax = fmin(t1, tmax);
		if (tmax <= tmin)
			return (false);
	}
	return (true);
}

t_aabb	surrounding_box(t_aabb box0, t_aabb box1)
{
	t_point	small = new_point(fmin(box0.min.x, box1.min.x),
							fmin(box0.min.y, box1.min.y),
							fmin(box0.min.z, box1.min.z));
	t_point	big = new_point(fmax(box0.max.x, box1.max.x),
							fmax(box0.max.y, box1.max.y),
							fmax(box0.max.z, box1.max.z));
	return (new_aabb(&small, &big));

}
