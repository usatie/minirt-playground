#include <stdlib.h>
#include "hittable.h"

bool	box_x_compare(t_hittable *a, t_hittable *b);
bool	box_y_compare(t_hittable *a, t_hittable *b);
bool	box_z_compare(t_hittable *a, t_hittable *b);
void	sort_hittable_list(t_hittable_list *s, t_hittable_list *e, t_comparator *comparator);
void	get_sphere_uv(const t_vec3 *p, double *u, double *v);

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

t_yz_rect	yzrect_new(double y0, double y1, double z0, double z1, double k, t_material *m)
{
	t_yz_rect	self = {};

	self.type = YZ_RECT;
	self.y0 = y0;
	self.y1 = y1;
	self.z0 = z0;
	self.z1 = z1;
	self.k = k;
	self.mat_ptr = m;
	return (self);
}

t_xz_rect	xzrect_new(double x0, double x1, double z0, double z1, double k, t_material *m)
{
	t_xz_rect	self = {};

	self.type = XZ_RECT;
	self.x0 = x0;
	self.x1 = x1;
	self.z0 = z0;
	self.z1 = z1;
	self.k = k;
	self.mat_ptr = m;
	return (self);
}

t_box		box_new(const t_point *p0, const t_point *p1, t_material *ptr)
{
	t_box	self = {};

	self.type = BOX;
	self.box_min = *p0;
	self.box_max = *p1;
	self.sides = hittable_list_alloc();

	hittable_list_add(self.sides, xyrect_alloc(p0->x, p1->x, p0->y, p1->y, p1->z, ptr));
	hittable_list_add(self.sides, xyrect_alloc(p0->x, p1->x, p0->y, p1->y, p0->z, ptr));

	hittable_list_add(self.sides, xzrect_alloc(p0->x, p1->x, p0->z, p1->z, p1->y, ptr));
	hittable_list_add(self.sides, xzrect_alloc(p0->x, p1->x, p0->z, p1->z, p0->y, ptr));

	hittable_list_add(self.sides, yzrect_alloc(p0->y, p1->y, p0->z, p1->z, p1->x, ptr));
	hittable_list_add(self.sides, yzrect_alloc(p0->y, p1->y, p0->z, p1->z, p0->x, ptr));
	return (self);
}

t_hittable_list	hittable_list_new(void)
{
	t_hittable_list	self = {};

	self.type = HITTABLE_LIST;
	return (self);
}

t_bvh_node	new_bvh_node(t_hittable_list *s, t_hittable_list *e)
{
	t_bvh_node	self = {};
	t_comparator	*comp[3] = {box_x_compare, box_y_compare, box_z_compare};

	self.type = BVH_NODE;
	int	axis = random_int_range(0, 2);
	size_t	span = span_of_hittable_list(s, e);
	if (span == 1)
	{
		self.left = s;
		self.right = s;
	}
	else if (span == 2)
	{
		if (comp[axis](s, s->next))
		{
			self.left = s;
			self.right = s->next;
		}
		else
		{
			self.left = s->next;
			self.right = s;	
		}
	}
	else
	{
		sort_hittable_list(s, e, comp[axis]);
		t_hittable_list *mid = s;
		for (size_t i = 0; i < span / 2; i++)
			mid = mid->next;
		self.left = alloc_bvh_node(s, mid);
		self.right = alloc_bvh_node(mid, e);
	}
	t_aabb	box_left, box_right;

	if (!bounding_box(self.left, &box_left) || !bounding_box(self.right, &box_right))
		dprintf(2, "No bounding box in bvh_node constructor.\n");
	self.box = surrounding_box(box_left, box_right);
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


t_yz_rect	*yzrect_alloc(double y0, double y1, double z0, double z1, double k, t_material *m)
{
	t_xy_rect	*self;

	self = calloc(1, sizeof(*self));
	*self = yzrect_new(y0, y1, z0, z1, k, m);
	return (self);
}


t_xz_rect	*xzrect_alloc(double x0, double x1, double z0, double z1, double k, t_material *m)
{
	t_xz_rect	*self;

	self = calloc(1, sizeof(*self));
	*self = xzrect_new(x0, x1, z0, z1, k, m);
	return (self);
}

t_box		*box_alloc(const t_point *p0, const t_point *p1, t_material *m)
{
	t_box	*self;

	self = calloc(1, sizeof(*self));
	*self = box_new(p0, p1, m);
	return (self);
}

t_hittable_list	*hittable_list_alloc(void)
{
	t_hittable_list	*self;

	self = calloc(1, sizeof(*self));
	*self= hittable_list_new();
	return (self);
}

t_bvh_node	*alloc_bvh_node(t_hittable_list *s, t_hittable_list *e)
{
	t_bvh_node	*self;

	self = calloc(1, sizeof(*self));
	*self = new_bvh_node(s, e);
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
			t_vec3	tmp = scalar_div_vec3(sub_vec3(rec->p, self->center), self->radius);
			get_sphere_uv(&tmp, &rec->u, &rec->v);
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

bool	yzrect_hit(const t_xy_rect *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	double	t = (self->k - r->origin.x) / r->direction.x;
	if (t < t_min || t > t_max)
		return (false);
	double	z = r->origin.z + t * r->direction.z;
	double	y = r->origin.y + t * r->direction.y;
	if (z < self->z0 || z > self->z1 || y < self->y0 || y > self->y1)
		return (false);
	rec->u = (z - self->z0) / (self->z1 - self->z0);
	rec->v = (y - self->y0) / (self->y1 - self->y0);
	rec->t = t;
	t_vec3	outward_normal = new_vec3(1, 0, 0);
	set_face_normal(rec, r, &outward_normal);
	rec->mat_ptr = self->mat_ptr;
	rec->p = ray_at(r, t);
	return (true);
}

bool	xzrect_hit(const t_xy_rect *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	double	t = (self->k - r->origin.y) / r->direction.y;
	if (t < t_min || t > t_max)
		return (false);
	double	x = r->origin.x + t * r->direction.x;
	double	z = r->origin.z + t * r->direction.z;
	if (x < self->x0 || x > self->x1 || z < self->z0 || z > self->z1)
		return (false);
	rec->u = (x - self->x0) / (self->x1 - self->x0);
	rec->v = (z - self->z0) / (self->z1 - self->z0);
	rec->t = t;
	t_vec3	outward_normal = new_vec3(0, 1, 0);
	set_face_normal(rec, r, &outward_normal);
	rec->mat_ptr = self->mat_ptr;
	rec->p = ray_at(r, t);
	return (true);
}

bool	box_hit(const t_xy_rect *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	return (hit(self->sides, r, t_min, t_max, rec));
}

bool	bvh_node_hit(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	if (!hit_aabb(&(self->box), r, t_min, t_max))
		return (false);
	bool	hit_left = hit(self->left, r, t_min, t_max, rec);
	bool	hit_right = hit(self->right, r, t_min, hit_left ? rec->t : t_max, rec);
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
	else if (self->type == YZ_RECT)
		return (yzrect_hit(self, r, t_min, t_max, rec));
	else if (self->type == XZ_RECT)
		return (xzrect_hit(self, r, t_min, t_max, rec));
	else if (self->type == BOX)
		return (box_hit(self, r, t_min, t_max, rec));
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

#define AABB_MIN_WIDTH 0.0001
bool	xyrect_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	t_vec3	a, b;

	a = new_point(self->x0, self->y0, self->k - AABB_MIN_WIDTH);
	b = new_point(self->x1, self->y1, self->k + AABB_MIN_WIDTH);
	*output_box = new_aabb(&a, &b);
	return (true);
}

bool	yzrect_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	t_vec3	a, b;

	a = new_point(self->k - AABB_MIN_WIDTH, self->y0, self->z0);
	b = new_point(self->k + AABB_MIN_WIDTH, self->y1, self->z1);
	*output_box = new_aabb(&a, &b);
	return (true);
}

bool	xzrect_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	t_vec3	a, b;

	a = new_point(self->x0, self->k - AABB_MIN_WIDTH,  self->z0);
	b = new_point(self->x1, self->k + AABB_MIN_WIDTH, self->z1);
	*output_box = new_aabb(&a, &b);
	return (true);
}

bool	box_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	*output_box = new_aabb(&self->box_min, &self->box_max);
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
	else if (self->type == XY_RECT)
		return (xyrect_bounding_box(self, output_box));
	else if (self->type == YZ_RECT)
		return (yzrect_bounding_box(self, output_box));
	else if (self->type == XZ_RECT)
		return (xzrect_bounding_box(self, output_box));
	else if (self->type == BOX)
		return (box_bounding_box(self, output_box));
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

bool	box_x_compare(t_hittable *a, t_hittable *b)
{
	t_aabb	box_a;
	t_aabb	box_b;

	if (!bounding_box(a, &box_a) || !bounding_box(b, &box_b))
		printf("no bounding box in bvh_node constructor\n");
	return (box_a.min.x < box_b.min.x);
}

bool	box_y_compare(t_hittable *a, t_hittable *b)
{
	t_aabb	box_a;
	t_aabb	box_b;

	if (!bounding_box(a, &box_a) || !bounding_box(b, &box_b))
		printf("no bounding box in bvh_node constructor\n");
	return (box_a.min.y < box_b.min.y);
}

bool	box_z_compare(t_hittable *a, t_hittable *b)
{
	t_aabb	box_a;
	t_aabb	box_b;

	if (!bounding_box(a, &box_a) || !bounding_box(b, &box_b))
		printf("no bounding box in bvh_node constructor\n");
	return (box_a.min.z < box_b.min.z);
}

//typedef bool	t_int_comparator(int a, int b);

// int	arr[3] = {3, 2, 1};
//
// sort(&arr[0], &arr[2], int_comparator);
// [2, 1]
//  ^  ^
//  s  e
/*
void	swap(int *a, int *b)
{
	int	tmp = *a;
	*a = *b;
	*b = tmp;
}

typedef struct s_list {
	int content;
	struct s_list *next;
}	t_list;
*/

// a->b->c
// 5->3->2
//
// tmp = a->val;
// a->val = c->val;
// c->val = tmp;
static void	swap_hittable_list(t_hittable_list *a, t_hittable_list *b)
{
	t_hittable_list	tmp = *a;
	t_hittable_list	*next;

	// a = b
	next = a->next;
	*a = *b;
	a->next = next;

	// b = tmp
	next = b->next;
	*b = tmp;
	b->next = next;
}

void	sort_hittable_list(t_hittable_list *s, t_hittable_list *e, t_comparator *comparator)
{
	for (t_hittable_list *i = s; i; i = i->next) {
		if (i == e) break;
		for (t_hittable_list *j = i->next; j; j = j->next) {
			if (!comparator(i, j))
				swap_hittable_list(i, j);
			if (j == e) break;
		}
	}
}
/*
void	sort(int *s, int *e, t_int_comparator *comparator)
{
	for (int *i = s; 1; i++)
	{
		if (i == e) break;
		for (int *j = s + 1; 1; j++)
		{
			if (!comparator(*i, *j))
				swap(i, j);
			if (j == e)
				break;
		}
	}
}
*/

/*
void	sort_hittable_list(t_hittable_list **start, t_hittable_list **end, t_comparator *comparator)
{
	t_hittable_list	*s;
	t_hittable_list	*c;
	t_hittable_list	*tmp;
	t_hittable_list	*e;

	s = *start;
	c = (*start)->next;

	while (s != *end)
	{
		while (c != *end)
		{
			if (!comparator(s, c))
		}
	}
}
*/

size_t	span_of_hittable_list(t_hittable_list *start, t_hittable_list *end)
{
	size_t	span = 0;

	for (t_hittable *i = start; i != end; i = i->next)
	{
		span++;
	}
	return (span);
}
