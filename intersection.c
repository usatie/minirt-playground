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

t_intersection_point	*test_intersection(t_shape *shape, t_ray *ray)
{
	t_intersection_point	*intersection;

	if (shape->kind == SPHERE)
		intersection = sphere_test_intersection(shape, ray);
	else if (shape->kind == PLANE)
		intersection = plane_test_intersection(shape, ray);
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
