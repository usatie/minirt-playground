#include <stdlib.h>
#include "hittable.h"

// new
t_sphere	sphere_new(t_point cen, double r, t_material *m)
{
	t_sphere	self = {};

	self.type = SPHERE;
	self.center = cen;
	self.radius = r;
	self.mat_ptr = m;
	self.next = NULL;
	return (self);
}

t_xy_rect	xyrect_new(double x0, double x1, double y0, double y1, double k, t_material *m)
{
	t_xy_rect	self = {};

	self.type = XY_RECT;
	self.x0 = x0;
	self.x1 = x1;
	self.y0 = y0;
	self.y1 = y1;
	self.k = k;
	self.mat_ptr = m;
	return (self);
}

t_hittable_list	hittable_list_new(void)
{
	t_hittable_list	self = {};

	self.type = HITTABLE_LIST;
	return (self);
}

t_bvh_node	new_bvh_node(t_hittable_list *list)
{
	t_bvh_node	self = {};

	self.type = BVH_NODE;
	self = *list;
	return (self);
}

// alloc
t_sphere	*sphere_alloc(t_point cen, double r, t_material *m)
{
	t_sphere	*self;

	self = calloc(1, sizeof(*self));
	*self = sphere_new(cen, r, m);
	return (self);
}

t_xy_rect	*xyrect_alloc(double x0, double x1, double y0, double y1, double k, t_material *m)
{
	t_xy_rect	*self;

	self = calloc(1, sizeof(*self));
	*self = xyrect_new(x0, x1, y0, y1, k, m);
	return (self);
}

t_hittable_list	*hittable_list_alloc(void)
{
	t_hittable_list	*self;

	self = calloc(1, sizeof(*self));
	*self= hittable_list_new();
	return (self);
}

t_bvh_node	*alloc_bvh_node(t_hittable_list *list)
{
	t_bvh_node	*self;

	self = calloc(1, sizeof(*self));
	*self = new_bvh_node(list);
	return (self);

}

// hit
bool	sphere_hit(const t_sphere *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	t_vec3	oc = sub_vec3(r->origin, self->center);
	double a  =length_squared_vec3(r->direction);
	double	half_b = dot_vec3(oc, r->direction);
	double c = length_squared_vec3(oc) - self->radius * self->radius;
	double discriminant = half_b * half_b - a * c;
	if (discriminant > 0)
	{
		double	root = sqrt(discriminant);
		double	temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min)
		{
			rec->t = temp;
			rec->p = ray_at(r, rec->t);
			t_vec3 outward_normal = scalar_div_vec3(sub_vec3(rec->p, self->center), self->radius);
			set_face_normal(rec, r, &outward_normal);
			rec->mat_ptr = self->mat_ptr;
			return (true);
		}
		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min)
		{
			rec->t = temp;
			rec->p = ray_at(r, rec->t);
			t_vec3 outward_normal = scalar_div_vec3(sub_vec3(rec->p, self->center), self->radius);
			set_face_normal(rec, r, &outward_normal);
			rec->mat_ptr = self->mat_ptr;
			return (true);
		}
	}
	return (false);
}

bool	xyrect_hit(const t_xy_rect *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	double	t = (self->k - r->origin.z) / r->direction.z;
	if (t < t_min || t > t_max)
		return (false);
	double	x = r->origin.x + t * r->direction.x;
	double	y = r->origin.y + t * r->direction.y;
	if (x < self->x0 || x > self->x1 || y < self->y0 || y > self->y1)
		return (false);
	rec->u = (x - self->x0) / (self->x1 - self->x0);
	rec->v = (y - self->y0) / (self->y1 - self->y0);
	rec->t = t;
	t_vec3	outward_normal = new_vec3(0, 0, 1);
	set_face_normal(rec, r, &outward_normal);
	rec->mat_ptr = self->mat_ptr;
	rec->p = ray_at(r, t);
	return (true);
}

bool	bvh_node_hit(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
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

	for (const t_hittable *object = self->next; object; object = object->next)
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
		return (sphere_hit(self, r, t_min, t_max, rec));
	else if (self->type == XY_RECT)
		return (xyrect_hit(self, r, t_min, t_max, rec));
	else if (self->type == BVH_NODE)
		return (bvh_node_hit(self, r, t_min, t_max, rec));
	else if (self->type == HITTABLE_LIST)
		return (hittable_list_hit(self, r, t_min, t_max, rec));
	else
		return (false);
}

// bounding_box
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

// hittable_list
void	hittable_list_add(t_hittable_list *self, t_hittable *object)
{
	while (self->next)
		self = self->next;
	self->next = object;
}

// set_face_normal
void	set_face_normal(t_hit_record *self, const t_ray *r, const t_vec3 *outward_normal)
{
	self->front_face = dot_vec3(r->direction, *outward_normal) < 0;
	if (self->front_face)	
		self->normal = *outward_normal;
	else
		self->normal = scalar_mul_vec3(-1.0, *outward_normal);
}

// get_sphere_uv
void	get_sphere_uv(const t_vec3 *p, double *u, double *v)
{
	double	phi = atan2(p->z, p->x);
	double	theta = asin(p->y);
	*u = 1 - (phi + M_PI) / (2 * M_PI);
	*v = (theta + M_PI_2) / M_PI;
}
