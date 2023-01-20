#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "color.h"
#include "mlx.h"

t_sphere	*sphere_new(pvector *center, float diameter, float amb)
{
	t_sphere	*sp;

	sp = calloc(1, sizeof(*sp));
	sp->center = center;
	sp->diameter = diameter;
	sp->k_amb = amb;
	return (sp);
}

t_ray	*ray_new(pvector *start, pvector *direction)
{
	t_ray	*ray;

	ray = calloc(1, sizeof(*ray));
	ray->start = start;
	ray->direction = direction;
	return (ray);
}

int	sphere_intersect(t_sphere *sp, t_ray *ray, float *t1, float *t2)
{
	pvector *d = ray->direction;
	pvector *s = ray->start;
	pvector *pc = sp->center;
	float	r = sp->diameter;
	float	a = pvector_dot(d, d);
	float	b = 2.0 * pvector_dot(pvector_sub(s, pc), d);
	float	c = pvector_magsq(pvector_sub(s, pc)) - r * r;
	float	D = b * b - 4.0 * a * c;

	if (D > 0)
	{
		*t1 = (-b - sqrt(D)) / (2.0 * a);
		*t2 = (-b + sqrt(D)) / (2.0 * a);
		return (2);
	}
	else if (D == 0)
	{
		*t1 = *t2 = (-b) / (2.0 * a);
		return (1);
	}
	else
		return (0);
}

#define BG_COLOR 0x64aaee

t_ray *get_ray(int x, int y, pvector *camera)
{
	pvector *x_dir = pvector_new(1, 0, 0);
	pvector *y_dir = pvector_new(0, 1, 0);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1);
	float v = map(y, 0, WIN_WIDTH - 1, 1, -1);

	pvector *ray_dir = pvector_sub(pvector_add(pvector_mul(x_dir, u), pvector_mul(y_dir, v)), camera);
	return (ray_new(camera, ray_dir));
}

#define AMB_L 0.01

int	ambient_light(t_ray *ray, t_sphere *sphere)
{
	(void)ray;
	
	return (AMB_L * sphere->k_amb);
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

float	has_intersection(t_ray *ray, t_sphere *sp)
{
	pvector *d = ray->direction;
	pvector *s = ray->start;
	pvector *pc = sp->center;
	float	r = sp->diameter;
	float	a = pvector_dot(d, d);
	float	b = 2.0 * pvector_dot(pvector_sub(s, pc), d);
	float	c = pvector_magsq(pvector_sub(s, pc)) - r * r;
	float	D = b * b - 4.0 * a * c;

	if (D > 0)
	{
		float t1 = (-b - sqrt(D)) / (2.0 * a);
		float t2 = (-b + sqrt(D)) / (2.0 * a);
		if (t1 > 0)
			return (t1);
		return (t2);
	}
	else if (D == 0)
	{
		float t1 = (-b) / (2.0 * a);
		return (t1);
	}
	return (-1);
}

t_shape	*shape_new(t_shape_kind kind)
{
	t_shape	*sp;

	sp = calloc(1, sizeof(*sp));
	sp->kind = kind;
	return (sp);
}

int	main(void)
{
	t_env	e;
	
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	pvector *camera;
	pvector *light;
	t_sphere	*sphere;

	sphere = sphere_new(pvector_new(0, 0, 5), 1.0, 0.1);
	camera = pvector_new(0, 0, -5);
	light = pvector_new(-5, 5, -5);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			//bool	intersection;
			t_rgb	color = blue();
			//int		color = BG_COLOR;
			float t;
			t_ray *ray = get_ray(x, y, camera);
			t = has_intersection(ray, sphere);
			if (t > 0)
			{
				float	R = 0;
				R += ambient_light(ray, sphere);
				R += diffuse_light(ray, sphere, t, light);
				R += specular_light(ray, sphere, t, light);
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
