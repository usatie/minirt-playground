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

int	ambient_light(t_ray *ray, t_shape *shape)
{
	(void)ray;
	(void)shape;
	float	k_amb = 0.1;
	
	return (AMB_L * k_amb);
}

float	diffuse_light(t_ray *ray, t_sphere *sphere, float t, pvector *light)
{
	pvector	*intersection;
	pvector	*n;
	pvector	*ray_light;
	float	light_intensity = 1.0;
	float	k_diffuse = 0.69;

	intersection = pvector_add(ray->start, pvector_mul(ray->direction, t));
	n = pvector_sub(intersection, sphere->center);
	pvector_normalize(n);
	ray_light = pvector_sub(light, intersection);
	pvector_normalize(ray_light);
	return (light_intensity * k_diffuse * constrain(pvector_dot(ray_light, n), 0 , 1));
}

float	specular_light(t_ray *ray, t_sphere *sphere, float t, pvector *light)
{
	pvector	*intersection;
	pvector	*n;
	pvector	*incident_light;
	pvector	*reflection;
	pvector *b;
	pvector	*view;
	float	shineness = 128.0;
	float	k_spec = 0.3;
	float	light_intensity = 1.0;
	float	nldot;
	float	vrdot;

	intersection = pvector_add(ray->start, pvector_mul(ray->direction, t));
	n = pvector_sub(intersection, sphere->center);
	pvector_normalize(n);
	incident_light = pvector_sub(light, intersection);
	pvector_normalize(incident_light);
	nldot = pvector_dot(n, incident_light);
	b = pvector_mul(n, nldot * 2);
	reflection = pvector_sub(b, incident_light);
	view = pvector_mul(ray->direction, -1);
	pvector_normalize(view);
	vrdot = pvector_dot(view, reflection);
	if (nldot <= 0 || vrdot <= 0)
		return (0);
	return (k_spec * light_intensity * pow(vrdot, shineness));
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
	
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	pvector *camera;
	t_light_source *light_source;
	t_shape	*shape;

	shape = shape_new(SPHERE);
	shape->center = pvector_new(0, 0, 5);
	shape->radius = 1.0;
	camera = pvector_new(0, 0, -5);
	light_source = light_source_new(POINT);
	light_source->position = pvector_new(-5, 5, -5);
	light_source->intencity = pvector_new(1.0, 1.0, 1.0);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			t_rgb	color = blue();
			t_intersection_point	*intersection;
			t_ray *ray = get_ray(x, y, camera);
			intersection = test_intersection(shape, ray);
			if (intersection)
			{
				float	R = 0;
				R += ambient_light(ray, shape);
				R += diffuse_light(ray, shape, intersection->distance, light);
				R += specular_light(ray, shape, intersection->distance, light);
				color = red();
				color = rgb_mul(color, R);
			}
			put_pixel(e.screen->img, x, y, color.mlx_color);
		}
	}
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);
	mlx_loop(e.mlx_ptr);
	return (0);
}
