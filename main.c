#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "mlx.h"

t_screen	*init_screen(void *mlx_ptr)
{
	t_screen	*screen;

	screen = calloc(1, sizeof(*screen));
	screen->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "screen");
	screen->img = init_img(mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	return (screen);
}

t_sphere	*sphere_new(pvector *center, float diameter)
{
	t_sphere	*sp;

	sp = calloc(1, sizeof(*sp));
	sp->center = center;
	sp->diameter = diameter;
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
	pvector *y_dir = pvector_new(0, -1, 0);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1);
	float v = map(y, 0, WIN_WIDTH - 1, 1, -1);

	pvector *ray_dir = pvector_sub(pvector_add(pvector_mul(x_dir, u), pvector_mul(y_dir, v)), camera);
	return (ray_new(camera, ray_dir));
}

int	ambient_light(int x, int y, t_ray *ray, t_sphere *sphere)
{
	float t1, t2;
	(void)x;
	(void)y;
	
	if (sphere_intersect(sphere, ray, &t1, &t2) > 0)
	{
		if (t1 > 0 || t2 > 0)
			return(0xff0000);
	}
	return (0);
}

float	diffuse_light(t_ray *ray, t_sphere *sphere, float t, pvector *light)
{
	pvector	*intersection;
	pvector	*n;
	pvector	*ray_light;

	intersection = pvector_add(ray->start, pvector_mul(ray->direction, t));
	n = pvector_sub(intersection, sphere->center);
	pvector_normalize(n);
	ray_light = pvector_sub(light, intersection);
	pvector_normalize(ray_light);
	return (constrain(pvector_dot(ray_light, n), 0 , 1));
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
	return (-1);}

int	main(void)
{
	t_env	e;
	
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	pvector *camera;
	pvector *light;
	t_sphere	*sphere;

	sphere = sphere_new(pvector_new(0, 0, 5), 1.0);
	camera = pvector_new(0, 0, -5);
	light = pvector_new(-5, 5, -5);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			//bool	intersection;
			int		color = BG_COLOR;
			float t;
			t_ray *ray = get_ray(x, y, camera);
			t = has_intersection(ray, sphere);
			if (t > 0)
			{
				float	R = 0;
				//R += ambient_light(x, y, camera, sphere);
				R += diffuse_light(ray, sphere, t, light);
				//R += specular_light(x, y, camera, sphere);
				color = 255 * R;
			}
			put_pixel(e.screen->img, x, y, color);
		}
	}
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);
	mlx_loop(e.mlx_ptr);
	return (0);
}
