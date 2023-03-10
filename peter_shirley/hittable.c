#include <stdlib.h>
#include "hittable.h"
#include "material.h"

bool	box_x_compare(t_hittable *a, t_hittable *b);
bool	box_y_compare(t_hittable *a, t_hittable *b);
bool	box_z_compare(t_hittable *a, t_hittable *b);
void	sort_hittable_list(t_hittable_list *s, t_hittable_list *e, t_comparator *comparator);
void	get_sphere_uv(const t_vec3 *p, double *u, double *v);

// new
t_sphere	sphere_new(t_point cen, double r, t_material *m)
{
	t_sphere	self = {0};

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
		printf("hello1\n");
		self.left = s;
		self.right = s;
	}
	else if (span == 2)
	{
		printf("hello2\n");
		printf("s = %p, s->next = %p, axis = %d\n", s, s->next, axis);
		printf("s->next->next = %p\n", s->next->next);
		if (comp[axis](s, s->next))
		{
			printf("1\n");
			self.left = s;
			self.right = s->next;
		}
		else
		{
			printf("2\n");
			self.left = s->next;
			self.right = s;	
		}
	}
	else
	{
		printf("1\n");
		sort_hittable_list(s, e, comp[axis]);
		printf("2\n");
		t_hittable_list *mid = s;
		for (size_t i = 0; i < span / 2; i++)
			mid = mid->next;
		printf("3\n");
		self.left = alloc_bvh_node(s, mid);
		printf("4\n");
		printf("mid = %p, e = %p\n", mid, e);
		self.right = alloc_bvh_node(mid, e);
		printf("5\n");
	}
	t_aabb	box_left, box_right;

	if (!bounding_box(self.left, &box_left) || !bounding_box(self.right, &box_right))
		dprintf(2, "No bounding box in bvh_node constructor.\n");
	self.box = surrounding_box(box_left, box_right);
	return (self);
}

t_translate	translate_new(t_hittable *p, const t_vec3 *displacement)
{
	t_translate	self = {};

	self.type = TRANSLATE;
	self.ptr = p;
	self.offset = *displacement;
	return (self);
}

t_rotate_y		rotate_y_new(t_hittable *p, double angle)
{
	t_rotate_y	self = {};
	double		radians = degrees_to_radians(angle);

	self.type = ROTATE_Y;
	self.ptr = p;
	self.sin_theta = sin(radians);
	self.cos_theta = cos(radians);
	self.hasbox = bounding_box(self.ptr, &self.box);

	t_vec3	min = new_point(INFINITY, INFINITY, INFINITY);
	t_vec3	max = new_point(-INFINITY, -INFINITY, -INFINITY);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				double	x = i * self.box.max.x + (1-i) * self.box.min.x;
				double	y = j * self.box.max.x + (1-j) * self.box.min.x;
				double	z = k * self.box.max.x + (1-k) * self.box.min.x;
				double	newx = self.cos_theta * x + self.sin_theta * z;
				double	newz = -self.sin_theta * x + self.cos_theta * z;

				t_vec3	tester = new_point(newx, y, newz);
				min.x = fmin(min.x, tester.x);
				max.x = fmax(max.x, tester.x);
				min.y = fmin(min.y, tester.y);
				max.y = fmax(max.y, tester.y);
				min.z = fmin(min.z, tester.z);
				max.z = fmax(max.z, tester.z);
			}
		}
	}
	self.box = new_aabb(&min, &max);
	return (self);
}

t_const_medium	const_medium_new(t_hittable *b, double d, t_texture *a)
{
	t_const_medium	self = {};

	self.type = CONST_MEDIUM;
	self.boundary = b;
	self.neg_inv_density = -1.0 / d;
	self.phase_function = alloc_isotropic(a);
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

t_translate		*translate_alloc(t_hittable *p, const t_vec3 *displacement)
{
	t_translate	*self;

	self = calloc(1, sizeof(*self));
	*self = translate_new(p, displacement);
	return (self);
}

t_rotate_y		*rotate_y_alloc(t_hittable *p, double angle)
{
	t_rotate_y	*self;

	self = calloc(1, sizeof(*self));
	*self = rotate_y_new(p, angle);
	return (self);
}

t_const_medium	*const_medium_alloc(t_hittable *b, double d, t_texture *a)
{
	t_const_medium	*self;

	self = calloc(1, sizeof(*self));
	*self = const_medium_new(b, d, a);
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

bool		translate_hit(const t_translate *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	t_ray moved_r = new_ray(sub_vec3(r->origin, self->offset), r->direction);
	if (!hit(self->ptr, &moved_r, t_min, t_max, rec))
		return (false);
	rec->p = add_vec3(rec->p, self->offset);
	set_face_normal(rec, &moved_r, &rec->normal);
	return (true);
}

bool	rotate_y_hit(const t_translate *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	t_point	origin = r->origin;
	t_vec3	direction = r->direction;
	origin.x = self->cos_theta * r->origin.x - self->sin_theta * r->origin.z;
	origin.z = self->sin_theta * r->origin.x + self->cos_theta * r->origin.z;
	direction.x = self->cos_theta * r->direction.x - self->sin_theta * r->direction.z;
	direction.z = self->sin_theta * r->direction.x + self->cos_theta * r->direction.z;
	t_ray	rotated_r = new_ray(origin, direction);
	if (!hit(self->ptr, &rotated_r, t_min, t_max, rec))
		return (false);

	t_point	p = rec->p;
	t_vec3	normal = rec->normal;
	p.x = self->cos_theta * rec->p.x + self->sin_theta * rec->p.z;
	p.z = -self->sin_theta * rec->p.x + self->cos_theta * rec->p.z;
	normal.x = self->cos_theta * rec->normal.x + self->sin_theta * rec->normal.z;
	normal.z = -self->sin_theta * rec->normal.x + self->cos_theta * rec->normal.z;

	rec->p = p;
	set_face_normal(rec, &rotated_r, &normal);
	return (true);
}

bool		const_medium_hit(const t_const_medium *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
{
	t_hit_record rec1, rec2;

	if (!hit(self->boundary, r, -INFINITY, INFINITY, &rec1))
		return (false);
	if (!hit(self->boundary, r, rec1.t + 0.0001, INFINITY, &rec2))
		return (false);

	if (rec1.t < t_min) rec1.t = t_min;
	if (rec2.t > t_max) rec2.t = t_max;

	if (rec1.t >= rec2.t)
		return (false);
	if (rec1.t < 0)
		rec1.t = 0;
	const double ray_len = length_vec3(r->direction);
	const double distance_inside_boundary = (rec2.t - rec1.t) * ray_len;
	const double hit_distance = self->neg_inv_density * log(random_double());

	if (hit_distance > distance_inside_boundary)
		return (false);
	
	rec->t = rec1.t + hit_distance / ray_len;
	rec->p = ray_at(r, rec->t);

	rec->normal = new_vec3(1, 0, 0);
	rec->front_face = true;
	rec->mat_ptr = self->phase_function;

	return (true);

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
	else if (self->type == TRANSLATE)
		return (translate_hit(self, r, t_min, t_max, rec));
	else if (self->type == ROTATE_Y)
		return (rotate_y_hit(self, r, t_min, t_max, rec));
	else if (self->type == CONST_MEDIUM)
		return (const_medium_hit(self, r, t_min, t_max, rec));
	else
		return (false);
}

// bounding_box
bool	sphere_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	printf("sphere_bounding_box\n");
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

bool	translate_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	if (!bounding_box(self->ptr, output_box))
		return (false);
	t_point	p0 = add_vec3(output_box->min, self->offset);
	t_point	p1 = add_vec3(output_box->max, self->offset);
	*output_box = new_aabb(&p0, &p1);
	return (true);
}

bool	rotate_y_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	*output_box = self->box;
	return (self->hasbox);
}

bool	const_medium_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	printf("const_medium_bounding_box\n");
	printf("self = %p, self->boundary = %p\n", self, self->boundary);
	printf("self->boundary->type = %d\n", self->boundary->type);
	return (bounding_box(self->boundary, output_box));
}

bool	hittable_list_bounding_box(const t_hittable *self, t_aabb *output_box)
{
	if (self->next == NULL)
		return (false);
	t_aabb	temp_box;
	bool	first_box = true;
	for (t_hittable *object = self->next; object; object = object->next) {
		if (!bounding_box(object, &temp_box)) return false;
		*output_box = first_box ? temp_box : surrounding_box(*output_box, temp_box);
		first_box = false;
	}
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
	else if (self->type == TRANSLATE)
		return (translate_bounding_box(self, output_box));
	else if (self->type == ROTATE_Y)
		return (rotate_y_bounding_box(self, output_box));
	else if (self->type == CONST_MEDIUM)
		return (const_medium_bounding_box(self, output_box));
	else if (self->type == HITTABLE_LIST)
		return (hittable_list_bounding_box(self, output_box));
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

	printf("box_z_compare!\n");
	printf("a->type = %d, b->type = %d!\n", a->type, b->type);
	if (!bounding_box(a, &box_a) || !bounding_box(b, &box_b))
		printf("no bounding box in bvh_node constructor\n");
	printf("finish\n");
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
