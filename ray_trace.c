#include <stddef.h>
#include <math.h>
#include <stdio.h>
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
			t_ray		*re_ray;
			pvector		*p, *n, *v, *re, *re_ray_start;
			float		vndot;

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
		if (res->shape->material->use_refraction)
		{
			float		eta1, eta2, vndot;
			pvector		*p, *n, *v, *d;

			p = res->intersection_point->position;
			d = ray->direction;
			pvector_normalize(d);
			n = res->intersection_point->normal;
			v = pvector_mul(ray->direction, -1);
			pvector_normalize(v);
			vndot = pvector_dot(v, n);
			eta1 = scene->air_refraction;
			eta2 = res->shape->material->refraction;
			if (vndot < 0)
			{
				eta1 = res->shape->material->refraction;
				eta2 = scene->air_refraction;
				n = pvector_mul(n, -1);
				vndot = -vndot;
			}
			float	eta_r = eta2 / eta1;
			float	cos1 = vndot;
			float	cos2 = eta1 / eta2 * sqrt(pow(eta_r, 2) - (1 - pow(cos1, 2)));
			float	omega = eta_r * cos2 - cos1;
			pvector	*re = pvector_sub(pvector_mul(n, 2 * vndot), v);
			pvector	*fe = pvector_mul(pvector_sub(d, pvector_mul(n, omega)), eta1 / eta2);
			t_ray	*re_ray = ray_new(pvector_add(p, pvector_mul(re, C_EPSILON)), re);
			t_ray	*fe_ray = ray_new(pvector_add(p, pvector_mul(fe, C_EPSILON)), fe);

			t_fcolor	*R_re = ray_trace_recursive(scene, re_ray, recursion_level + 1);
			t_fcolor	*R_fe = ray_trace_recursive(scene, fe_ray, recursion_level + 1);

			float	rho_p = (eta_r * cos1 - cos2) / (eta_r * cos1 + cos2);
			float	rho_s = omega / (eta_r * cos2 + cos1);
			float	c_r = (pow(rho_p, 2) + pow(rho_s, 2)) / 2;
			float	c_t = 1 - c_r;
			t_fcolor	*catadioptric_factor = res->shape->material->catadioptric_factor;
			if (R_re != NULL)
			{
				t_fcolor *f_cr;
				t_fcolor	*R_reflection;

				f_cr = fcolor_new(c_r, c_r, c_r); 
				R_reflection = fcolor_mul(fcolor_mul(catadioptric_factor, f_cr) , R_re);
				R = fcolor_add(R, R_reflection);
			}
			if (R_fe != NULL)
			{
				t_fcolor *f_ct;
				t_fcolor	*R_refraction;

				f_ct = fcolor_new(c_t, c_t, c_t); 
				R_refraction = fcolor_mul(fcolor_mul(catadioptric_factor, f_ct) , R_fe);
				R = fcolor_add(R, R_refraction);
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
