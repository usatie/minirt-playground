#include <stdlib.h>
#include "hittable.h"
#include "sphere.h"

void	hittable_list_add(t_hittable_list *self, t_hittable *object)
{
	while (self->next)
		self = self->next;
	self->next = object;
}

bool	bvh_node_hit_single(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	if (!hit_aabb(&(self->box), r, t_min, t_max))
		return (false);
	bool	hit_left = hit(self->left, r, t_min, t_max, rec);
	bool	hit_right = hit(self->right, r, t_min, t_max, rec);
	return (hit_left || hit_right);
}

bool	hittable_list_hit(const t_hittable_list *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	bool			hit_anything = false;
	double			closest_so_far = t_max;

	for (const t_hittable *object = self; object; object = object->next)
	{
		if (hit(object, r, t_min, closest_so_far, &temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}
	return hit_anything;
}

bool	hit(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	if (self->type == SPHERE)
		return (sphere_hit_single(self, r, t_min, t_max, rec));
	else if (self->type == XY_RECT)
		return (xyrect_hit_single(self, r, t_min, t_max, rec));
	else if (self->type == BVH_NODE)
		return (bvh_node_hit_single(self, r, t_min, t_max, rec));
	else if (self->type == HITTABLE_LIST)
		return (hittable_list_hit(self, r, t_min, t_max, rec));
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

bool	bvh_node_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	*output_box = self->box;
	return (true);
}

bool	bounding_box(const t_hittable *self, t_aabb *output_box)
{
	if (self->type == SPHERE)
		return (sphere_bounding_box(self, output_box));
	else if (self->type == BVH_NODE)
		return (bvh_node_bounding_box(self, output_box));
	else
		return (false);
}

t_bvh_node	new_bvh_node(t_hittable_list *list)
{
	t_bvh_node	self;

	self = *list;
	return (self);
}

t_bvh_node	*alloc_bvh_node(t_hittable_list *list)
{
	t_bvh_node	*self;

	self = calloc(1, sizeof(*self));
	*self = new_bvh_node(list);
	return (self);

}
