#include <stddef.h>
#include "playground.h"

#define C_EPSILON (1.0 / 512.0)
t_ray	*get_shadow_ray(t_intersection_point *intersection, t_light_source *ls, float *distance_to_ls_loc)
{
	pvector	*ray_start;
	pvector	*ray_dir;

	ray_dir = pvector_sub(ls->position, intersection->position);
	*distance_to_ls_loc = pvector_mag(ray_dir) - C_EPSILON;
	pvector_normalize(ray_dir);
	ray_start = pvector_add(intersection->position, pvector_mul(ray_dir, C_EPSILON));
	return (ray_new(ray_start, ray_dir));
}

#define MAX_RECURSION 8

t_fcolor	*ray_trace_recursive(t_scene *scene, t_ray *ray, int recursion_level)
{
	t_fcolor					*R;
	t_intersection_test_result	*res;
	t_lighting					*lighting;

	if (recursion_level > MAX_RECURSION)
		return (NULL);
	res = test_intersection_with_all(scene, ray);
	if (res)
	{
		R = ambient_light(ray, res->shape);
		for (t_light_source *ls = scene->light_sources; ls; ls = ls->next)
		{
			t_ray	*shadow_ray;
			float	distance_to_ls;

			shadow_ray = get_shadow_ray(res->intersection_point, ls, &distance_to_ls);
			if (test_intersection_with_all_overload(scene, shadow_ray, distance_to_ls, true))
				continue ;
			lighting = lighting_at(res->intersection_point->position, ls);
			R = fcolor_add(R, diffuse_light(res->shape, res->intersection_point, lighting));
			R = fcolor_add(R, specular_light(ray, res->shape,res->intersection_point, lighting));
		}
		if (res->shape->material->use_perfect_reflectance)
		{
			t_fcolor	*R_re, *R_mirror;
			t_ray						*re_ray;
			pvector	*p, *n, *v, *re, *re_ray_start;
			float	vndot;

			p = res->intersection_point->position;
			n = res->intersection_point->normal;
			v = pvector_mul(ray->direction, -1);
			pvector_normalize(v);
			vndot = pvector_dot(v, n);
			re = pvector_sub(pvector_mul(n, 2 * vndot), v);
			pvector_normalize(re);
			re_ray_start = pvector_add(p, pvector_mul(re, C_EPSILON));
			re_ray = ray_new(re_ray_start, re);
			R_re = ray_trace_recursive(scene, re_ray, recursion_level + 1);
			if (R_re)
			{
				R_mirror = fcolor_mul(res->shape->material->catadioptric_factor, R_re);
				R = fcolor_add(R, R_mirror);
			}
		}
		return (R);
	}
	else
		return (fcolor_new(100.0 / 255.0, 149.0 / 255.0, 237.0 / 255.0)); // background color
	return (R);
}

t_fcolor	*ray_trace(t_scene *scene, t_ray *ray)
{
	return (ray_trace_recursive(scene, ray, 0));
}
