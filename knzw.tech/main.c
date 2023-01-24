#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "color.h"
#include "mlx.h"

t_ray *get_ray(int x, int y, t_scene *scene)
{
	pvector *dx, *dy;
	float	theta = M_PI_2;
	float	aspect_r = (float)WIN_WIDTH / WIN_HEIGHT;
	pvector *ey = pvector_new(cos(theta), sin(theta), 0);
	pvector	*df = pvector_sub(scene->look_at, scene->eye_position);
	pvector_normalize(df);
	dx = pvector_cross(ey, df);
	dy = pvector_cross(df, dx);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1) * aspect_r;
	float v = map(y, 0, WIN_HEIGHT - 1, 1, -1);

	pvector	*pm = pvector_add(scene->eye_position, pvector_mul(df, scene->screen_distance));

	pvector *ray_dir = pvector_sub(
			pvector_add(pm, pvector_add(pvector_mul(dx, u), pvector_mul(dy, v))),
			scene->eye_position);
	pvector_normalize(ray_dir);
	return (ray_new(scene->eye_position, ray_dir));
}

#define	SCREEN_WIDTH 2.0

t_ray *get_ray_FOV(int x, int y, t_scene *scene)
{
	pvector *dx, *dy;
	pvector *ey = pvector_new(0, 1, 0);
	pvector	*df = pvector_sub(scene->look_at, scene->eye_position);
	float	aspect_r = (float)WIN_WIDTH / WIN_HEIGHT;
	scene->screen_distance = SCREEN_WIDTH * aspect_r / ( 2 * tan(scene->HFOV));
	pvector_normalize(df);
	dx = pvector_cross(ey, df);
	dy = pvector_cross(df, dx);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1) * aspect_r;
	float v = map(y, 0, WIN_HEIGHT - 1, 1, -1);

	pvector	*pm = pvector_add(scene->eye_position, pvector_mul(df, scene->screen_distance));

	pvector *ray_dir = pvector_sub(
			pvector_add(pm, pvector_add(pvector_mul(dx, u), pvector_mul(dy, v))),
			scene->eye_position);
	pvector_normalize(ray_dir);
	return (ray_new(scene->eye_position, ray_dir));
}


int	main(void)
{
	t_env	e;
	t_scene	*scene;

	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	scene = get_scene();
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			t_fcolor	*R;
			t_ray 		*ray;

			ray = get_ray(x, y, scene);
			R = ray_trace(scene, ray);
			put_pixel(e.screen->img, x, y, fcolor2rgb(R).mlx_color);
		}
	}
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);
	mlx_loop(e.mlx_ptr);
	return (0);
}
