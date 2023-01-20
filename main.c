#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "color.h"
#include "mlx.h"

t_ray	*ray_new(pvector *start, pvector *direction)
{
	t_ray	*ray;

	ray = calloc(1, sizeof(*ray));
	ray->start = start;
	ray->direction = direction;
	return (ray);
}

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
t_intersection_point	*test_intersection(t_shape *shape, t_ray *ray)
{
	if (shape->kind == SPHERE)
		return (sphere_test_intersection(shape, ray));
	else
		exit(1);
}

t_ray *get_ray(int x, int y, pvector *camera)
{
	pvector *x_dir = pvector_new(1, 0, 0);
	pvector *y_dir = pvector_new(0, 1, 0);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1);
	float v = map(y, 0, WIN_WIDTH - 1, 1, -1);

	pvector *ray_dir = pvector_sub(pvector_add(pvector_mul(x_dir, u), pvector_mul(y_dir, v)), camera);
	pvector_normalize(ray_dir);
	return (ray_new(camera, ray_dir));
}

#define AMB_L 0.01

t_fcolor	*ambient_light(t_ray *ray, t_shape *shape)
{
	(void)ray;
	(void)shape;
	float	R;
	
	float	k_amb = 0.1;
	R = k_amb * AMB_L;
	return (fcolor_new(R, R, R));
}

t_fcolor	*diffuse_light(t_sphere *sphere, t_intersection_point *intersection, t_lighting *lighting)
{
	t_fcolor	*color;
	pvector	*n;
	pvector	*l;
	t_fcolor	*k_diffuse = fcolor_new(0.69, 0, 0);
	float	nldot;

	(void)sphere;
	n = intersection->normal;
	l = lighting->direction;
	nldot = constrain(pvector_dot(l, n), 0 , 1);
	color = fcolor_mul(lighting->intencity, k_diffuse);
	color = fcolor_mul(color, fcolor_new(nldot, nldot, nldot));
	return (color);
}

t_fcolor	*specular_light(t_ray *ray, t_sphere *sphere, t_intersection_point *intersection, t_lighting *lighting)
{
	pvector	*reflection;
	pvector *b;
	pvector	*view;
	t_fcolor	*fcolor;
	float	shineness = 128.0;
	t_fcolor	*k_spec = fcolor_new(0.3, 0.3, 0.3);
	float	nldot;
	float	vrdot;

	nldot = pvector_dot(intersection->normal, lighting->direction);
	b = pvector_mul(intersection->normal, nldot * 2);
	reflection = pvector_sub(b, lighting->direction);
	view = pvector_mul(ray->direction, -1);
	pvector_normalize(view);
	vrdot = pvector_dot(view, reflection);
	if (nldot <= 0 || vrdot <= 0)
		return (fcolor_new(0, 0, 0));
	float R = pow(vrdot, shineness);

	return (fcolor_mul(k_spec, fcolor_mul(lighting->intencity, fcolor_new(R, R, R))));
}

t_shape	*shape_new(t_shape_kind kind)
{
	t_shape	*sp;

	sp = calloc(1, sizeof(*sp));
	sp->kind = kind;
	return (sp);
}

t_light_source *light_source_new(t_light_kind kind)
{
	t_light_source *src;

	src = calloc(1, sizeof(*src));
	src->kind = kind;
	return (src);
}

t_lighting *lighting_at(pvector *pos, t_light_source *light_source)
{
	t_lighting	*lighting;

	lighting = calloc(1, sizeof(*lighting));
	lighting->direction = pvector_sub(light_source->position, pos);
	lighting->distance = pvector_mag(lighting->direction);
	pvector_normalize(lighting->direction);
	lighting->intencity = light_source->intencity;
	return (lighting);
}


int	main(void)
{
	t_env	e;
	pvector 		*camera;
	t_light_source	*light_source;
	t_lighting		*lighting;
	t_shape			*shape;
	
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);

	shape = shape_new(SPHERE);
	shape->center = pvector_new(0, 0, 5);
	shape->radius = 1.0;
	camera = pvector_new(0, 0, -5);
	light_source = light_source_new(POINT);
	light_source->position = pvector_new(-5, 5, -5);
	light_source->intencity = fcolor_new(1.0, 1.0, 1.0);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			t_rgb					color;
			t_intersection_point	*intersection;
			t_ray 					*ray;

			color = blue();
			ray = get_ray(x, y, camera);
			intersection = test_intersection(shape, ray);
			if (intersection)
			{
				t_fcolor	*R;
				t_fcolor	*fcolor;

				R = fcolor_new(0, 0, 0);
				lighting = lighting_at(intersection->position, light_source);
				R = fcolor_add(R, ambient_light(ray, shape));
				R = fcolor_add(R, diffuse_light(shape, intersection, lighting));
				R = fcolor_add(R, specular_light(ray, shape, intersection, lighting));
				fcolor = fcolor_new(1.0, 0.0, 0.0);
				fcolor = fcolor_mul(fcolor, R);
				color = fcolor2rgb(fcolor);
			}
			put_pixel(e.screen->img, x, y, color.mlx_color);
		}
	}
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);
	mlx_loop(e.mlx_ptr);
	return (0);
}
