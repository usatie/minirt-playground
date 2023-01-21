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

t_intersection_test_result	*test_intersection_with_all(t_scene *scene, t_ray *ray)
{
	t_intersection_test_result	*res;

	res = calloc(1, sizeof(*res));
	for (t_shape *shape = scene->shapes; shape; shape = shape->next)
	{
		t_intersection_point	*intersection;

		intersection = test_intersection(shape, ray);
		if (intersection == NULL)
			continue ;
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
	}
	return (res);
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

t_fcolor	*ambient_light(t_ray *ray, t_shape *shape)
{
	t_fcolor	*ambient_intensity;
	
	(void)ray;
	ambient_intensity = fcolor_new(0.1, 0.1, 0.1);
	return (fcolor_mul(ambient_intensity, shape->material->amibient_factor));
}

t_fcolor	*diffuse_light(t_shape *shape, t_intersection_point *intersection, t_lighting *lighting)
{
	t_fcolor	*color;
	pvector	*n;
	pvector	*l;
	float	nldot;

	n = intersection->normal;
	l = lighting->direction;
	nldot = constrain(pvector_dot(l, n), 0 , 1);
	color = fcolor_mul(lighting->intencity, shape->material->diffuse_factor);
	color = fcolor_mul(color, fcolor_new(nldot, nldot, nldot));
	return (color);
}

t_fcolor	*specular_light(t_ray *ray, t_shape *shape, t_intersection_point *intersection, t_lighting *lighting)
{
	pvector	*reflection;
	pvector *b;
	pvector	*view;
	float	nldot;
	float	vrdot;
	float	vrdot_pow_alpha;
	t_fcolor	*ki;

	nldot = pvector_dot(intersection->normal, lighting->direction);
	b = pvector_mul(intersection->normal, nldot * 2);
	reflection = pvector_sub(b, lighting->direction);
	view = pvector_mul(ray->direction, -1);
	pvector_normalize(view);
	vrdot = pvector_dot(view, reflection);
	if (nldot <= 0 || vrdot <= 0)
		return (fcolor_new(0, 0, 0));
	vrdot_pow_alpha = pow(vrdot, shape->material->shineness);
	ki = fcolor_mul(shape->material->specular_factor, lighting->intencity);
	return (fcolor_mul(ki, fcolor_new(vrdot_pow_alpha, vrdot_pow_alpha, vrdot_pow_alpha)));
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

t_material	*get_default_material(t_fcolor	*color)
{
	t_material		*material;

	material = material_new();
	material->amibient_factor = fcolor_new(0.01, 0.01, 0.01);
	material->diffuse_factor = color;
	material->specular_factor = fcolor_new(0.30, 0.30, 0.30);
	material->shineness = 8.0;
	return (material);
}

t_shape	*get_shapes(void)
{
	t_shape		head;
	t_shape		*shape;
	
	head.next = NULL;
	shape = &head;
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(3, 0, 25);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0.69, 0, 0));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(2, 0, 20);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0, 0.69, 0));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(1, 0, 15);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0, 0, 0.69));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(0, 0, 10);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0, 0.69, 0.69));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(-1, 0, 5);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0.69, 0, 0.69));
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->center = pvector_new(0, -1, 0);
		shape->normal = pvector_new(0, 1, 0);
		shape->material = get_default_material(fcolor_new(0.69, 0.69, 0.69));
	}
	return (head.next);
}

t_fcolor	*ray_trace(t_scene *scene, t_ray *ray)
{
	t_fcolor	*R;
	t_intersection_test_result	*res;
	t_lighting		*lighting;

	R = fcolor_new(0, 0, 1.0);
	res = test_intersection_with_all(scene, ray);
	if (res->intersection_point)
	{
		R = fcolor_new(0, 0, 0);
		for (t_light_source *ls = scene->light_sources; ls; ls = ls->next)
		{
			lighting = lighting_at(res->intersection_point->position, ls);
			R = fcolor_add(R, ambient_light(ray, res->shape));
			R = fcolor_add(R, diffuse_light(res->shape, res->intersection_point, lighting));
			R = fcolor_add(R, specular_light(ray, res->shape,res->intersection_point, lighting));
		}
	}
	return (R);
}

t_light_source	*get_light_sources(void)
{
	t_light_source	head;
	t_light_source	*ls;

	head.next = NULL;
	ls = &head;
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(-5, 5, -5);
		ls->intencity = fcolor_new(0.5, 0.5, 0.5);
	}
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(5, 0, -5);
		ls->intencity = fcolor_new(0.5, 0.5, 0.5);
	}
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(5, 20, -5);
		ls->intencity = fcolor_new(0.5, 0.5, 0.5);
	}
	return (head.next);
}

t_scene	*get_scene(void)
{
	t_scene	*scene;

	scene = calloc(1, sizeof(*scene));
	scene->shapes = get_shapes();
	scene->light_sources = get_light_sources();
	return (scene);
}

int	main(void)
{
	t_env	e;
	pvector *camera;
	t_scene	*scene;

	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	scene = get_scene();
	camera = pvector_new(0, 0, -5);
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			t_fcolor	*R;
			t_ray 		*ray;

			ray = get_ray(x, y, camera);
			R = ray_trace(scene, ray);
			put_pixel(e.screen->img, x, y, fcolor2rgb(R).mlx_color);
		}
	}
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);
	mlx_loop(e.mlx_ptr);
	return (0);
}
