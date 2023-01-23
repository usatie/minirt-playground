#include <stdlib.h>
#include <math.h>
#include "playground.h"

t_intersection_point	*sphere_get_intersection(float t, t_ray *ray, t_sphere *sphere)
{
	pvector					*position;
	t_intersection_point	*intersection;

	position = pvector_add(ray->start, pvector_mul(ray->direction, t));
	intersection = calloc(1, sizeof(*intersection));
	intersection->distance = t;
	intersection->position = position;
	intersection->normal = pvector_sub(position, sphere->center);
	pvector_normalize(intersection->normal);
	return (intersection);
}

t_intersection_point	*sphere_test_intersection(t_sphere *sp, t_ray *ray)
{
	pvector *d = ray->direction;
	pvector *s = ray->start;
	pvector *pc = sp->center;
	float	r = sp->radius;
	float	a = pvector_dot(d, d);
	float	b = 2.0 * pvector_dot(pvector_sub(s, pc), d);
	float	c = pvector_magsq(pvector_sub(s, pc)) - r * r;
	float	D = b * b - 4.0 * a * c;
	float	t1, t2;

	if (D > 0)
	{
		t1 = (-b - sqrt(D)) / (2.0 * a);
		t2 = (-b + sqrt(D)) / (2.0 * a);
		if (t1 > 0)
			return (sphere_get_intersection(t1, ray, sp));
		else if (t2 > 0)
			return (sphere_get_intersection(t2, ray, sp));
	}
	else if (D == 0)
	{
		t1 = (-b) / (2.0 * a);
		if (t1 > 0)
			return (sphere_get_intersection(t1, ray, sp));
	}
	return (NULL);
}

t_intersection_point	*plane_get_intersection(float t, t_ray *ray, t_plane *plane)
{
	pvector					*position;
	t_intersection_point	*intersection;

	position = pvector_add(ray->start, pvector_mul(ray->direction, t));
	intersection = calloc(1, sizeof(*intersection));
	intersection->distance = t;
	intersection->position = position;
	intersection->normal = pvector_copy(plane->normal);
	pvector_normalize(intersection->normal);
	return (intersection);
}

t_intersection_point	*plane_test_intersection(t_plane *plane, t_ray *ray)
{
	pvector *d = ray->direction;
	pvector *s = ray->start;
	pvector *pc = plane->center;
	float	t;
	float	dndot;

	dndot = pvector_dot(d, plane->normal);
	if (dndot != 0)
	{
		t = pvector_dot(pvector_sub(pc, s), plane->normal) / dndot;
		if (t > 0)
			return (plane_get_intersection(t, ray, plane));
	}
	return (NULL);
}

t_intersection_point	*cylinder_get_intersection(float t, t_ray *ray, t_cylinder *cylinder)
{
	pvector					*position;
	t_intersection_point	*intersection;
	float					tmp;

	position = pvector_add(ray->start, pvector_mul(ray->direction, t));
	intersection = calloc(1, sizeof(*intersection));
	intersection->distance = t;
	intersection->position = position;

	tmp = pvector_dot(pvector_sub(position, cylinder->center), cylinder->orientation);
	intersection->normal = pvector_sub(
			position, 
			pvector_add(cylinder->center, pvector_mul(cylinder->orientation, tmp))
			);
	pvector_normalize(intersection->normal);
	return (intersection);
}

t_intersection_point	*cylinder_test_intersection(t_cylinder *cylinder, t_ray *ray)
{
	pvector *v = ray->direction;
	pvector *s = ray->start;
	pvector *pc = cylinder->center;
	float	radius = cylinder->radius;
	//float	height = cylinder->height;
	// p = s + td;
	// |p_xz - pc_xz|^2 = r * r
	// |(s + td)_xz - pc_xz|^2 = r * r
	// |(s - pc) + td|_xz ^ 2 = r*r
	// At^2 + Bt - C = 0
	// A = |d|^2
	// B = 2 * dot(s-pc, d)
	// C = |s-pc|^2 - r*r
	//
	//// |(s_x + t*d_x) - pc_x|^2 + |(s_z + t*d_z) - pc_z|^2 = r^2;

	pvector *w = pvector_sub(s, pc);
	pvector	*h = pvector_mul(cylinder->orientation, cylinder->height);
	float	A = pvector_magsq(v) - pow(pvector_dot(v, cylinder->orientation), 2);
	float	B = 2 * (pvector_dot(v, w) - pvector_dot(v, cylinder->orientation) * pvector_dot(w, cylinder->orientation));
	float	C = pvector_magsq(w) - pow(pvector_dot(w, cylinder->orientation) , 2) - pow(radius, 2);
	float	D = pow(B, 2) - 4 * A * C;
	float	t1, t2;
	

	// d->y = 0;
	// pvector	*s_sub_pc = pvector_sub(s, pc);
	// s_sub_pc->y = 0;
	// float	A = pvector_magsq(d);
	// float	B = 2 * pvector_dot(s_sub_pc, d);
	// float	C = pvector_magsq(s_sub_pc) - pow(radius, 2);
	// float	D = pow(B, 2) - 4 * A * C;
	// float	t1, t2;

	if (D > 0)
	{
		t1 = (-B - sqrt(D)) / (2.0 * A);
		t2 = (-B + sqrt(D)) / (2.0 * A);
		pvector *L1 = pvector_add(s, pvector_mul(v, t1));
		pvector *L2 = pvector_add(s, pvector_mul(v, t2));
		pvector *L1_sub_pc = pvector_sub(L1, pc);
		pvector *L2_sub_pc = pvector_sub(L2, pc);
		if (t1 > 0 && fabsf(pvector_dot(L1_sub_pc, h)) <= pvector_mag(h))
			return (cylinder_get_intersection(t1, ray, cylinder));
		else if (t2 > 0 && fabsf(pvector_dot(L2_sub_pc, h)) <= pvector_mag(h))
			return (cylinder_get_intersection(t2, ray, cylinder));
	}
	else if (D == 0)
	{
		t1 = (-B) / (2.0 * A);
		if (t1 > 0)
			return (cylinder_get_intersection(t1, ray, cylinder));
	}
	return (NULL);
}

t_intersection_point	*test_intersection(t_shape *shape, t_ray *ray)
{
	t_intersection_point	*intersection;

	if (shape->kind == SPHERE)
		intersection = sphere_test_intersection(shape, ray);
	else if (shape->kind == PLANE)
		intersection = plane_test_intersection(shape, ray);
	else if (shape->kind == CYLINDER)
		intersection = cylinder_test_intersection(shape, ray);
	else
		exit(1);
	return (intersection);
}

#include <float.h>
t_intersection_test_result	*test_intersection_with_all(t_scene *scene, t_ray *ray)
{
	return (test_intersection_with_all_overload(scene, ray, FLT_MAX, false));
}

t_intersection_test_result	*test_intersection_with_all_overload(t_scene *scene, t_ray *ray, float max_dist, bool exit_once_found)
{
	t_intersection_test_result	*res;

	res = NULL;
	for (t_shape *shape = scene->shapes; shape; shape = shape->next)
	{
		t_intersection_point	*intersection;

		intersection = test_intersection(shape, ray);
		if (intersection == NULL)
			continue ;
		if (intersection->distance >= max_dist)
			continue ;
		if (res == NULL)
			res = calloc(1, sizeof(*res));
		if (res->intersection_point == NULL)
		{
			res->intersection_point = intersection;
			res->shape = shape;
		}
		else if (intersection->distance < res->intersection_point->distance)
		{
			res->intersection_point = intersection;
			res->shape = shape;
		}
		if (exit_once_found && res->intersection_point->distance < max_dist)
			return (res);
	}
	return (res);
}
