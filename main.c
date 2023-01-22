#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "color.h"
#include "mlx.h"

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

int	main(void)
{
	t_env	e;
	pvector *camera;
	t_scene	*scene;

	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	scene = get_scene2();
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
