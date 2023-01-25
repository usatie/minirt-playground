#include "sphere.h"
#include <stddef.h> //NULL
#include <stdlib.h> // calloc

t_sphere	sphere_new(t_point cen, double r, t_material *m)
{
	t_sphere	sphere;

	sphere.center = cen;
	sphere.radius = r;
	sphere.mat_ptr = m;
	sphere.next = NULL;
	return (sphere);
}

t_sphere	*sphere_alloc(t_point cen, double r, t_material *m)
{
	t_sphere	*sphere;

	sphere = calloc(1, sizeof(*sphere));
	*sphere = sphere_new(cen, r, m);
	return (sphere);
}

bool	sphere_hit_single(const t_sphere *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec)
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

void	set_face_normal(t_hit_record *self, const t_ray *r, const t_vec3 *outward_normal)
{
	self->front_face = dot_vec3(r->direction, *outward_normal) < 0;
	if (self->front_face)	
		self->normal = *outward_normal;
	else
		self->normal = scalar_mul_vec3(-1.0, *outward_normal);
}

void	get_sphere_uv(const t_vec3 *p, double *u, double *v)
{
	double	phi = atan2(p->z, p->x);
	double	theta = asin(p->y);
	*u = 1 - (phi + M_PI) / (2 * M_PI);
	*v = (theta + M_PI_2) / M_PI;
}