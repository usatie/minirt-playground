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
	pvector *ey = pvector_new(cos(theta), sin(theta), 0);
	pvector	*df = pvector_sub(scene->look_at, scene->eye_position);
	pvector_normalize(df);
	dx = pvector_cross(ey, df);
	dy = pvector_cross(df, dx);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1);
	float v = map(y, 0, WIN_WIDTH - 1, 1, -1);

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
	scene = get_scene2();
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
