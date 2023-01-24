#include "hittable_list.h"

void	add(t_hittable_list *self, t_hittable *object)
{
	while (self->next)
		self = self->next;
	self->next = object;
}

bool	hit(const t_hittable_list *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	bool			hit_anything = false;
	double			closest_so_far = t_max;

	for (const t_hittable *object = self; object; object = object->next)
	{
		if (hit_single(object, r, t_min, closest_so_far, &temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}
	return hit_anything;
}
