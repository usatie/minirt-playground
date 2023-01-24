#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "color.h"
#include "mlx.h"
#include <unistd.h>
#include <time.h>


t_vec3	inter_vec(t_vec3 a, t_vec3 b)
{
	return (sub_vec3(a, b));
}

void	test_vec(void)
{
	t_vec3 ret;

	ret = new_vec3(1.0, 1.0, 1.0);
	ret = add_vec3(inter_vec(ret, new_vec3(3, 3, 3)), ret);
	print_vec3(ret);
}

int	main(void)
{
	t_env	e;
	double	viewport_height = 2.0;
	double	viewport_width = ASPECT_RATIO * viewport_height;
	double	focal_length = 1.0;

	t_point	origin = {0, 0, 0};
	t_vec3	horizontal = new_vec3(viewport_width, 0, 0);
	t_vec3	vertical = new_vec3(0, viewport_height, 0);
	t_vec3	mean_horizontal_vertical = scalar_div_vec3(add_vec3(horizontal, vertical), 2.0);
	t_vec3	lower_left_corner = sub_vec3(sub_vec3(origin, mean_horizontal_vertical), new_vec3(0, 0, focal_length));

	printf("WIN_WIDTH = %d, WIN_HEIGHT %d\n", WIN_WIDTH, WIN_HEIGHT);
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	for (int j = WIN_HEIGHT - 1; j >=0;  --j)
	{
		int y = WIN_HEIGHT - j;
		dprintf(STDERR_FILENO, "\rscanlines remainings: %d", j);
		for (int i = 0; i < WIN_WIDTH; ++i)
		{
			double	u = (double)i / (WIN_WIDTH - 1);
			double	v = (double)j / (WIN_HEIGHT - 1);
			t_vec3	uv = add_vec3(scalar_mul_vec3(u, horizontal), scalar_mul_vec3(v, vertical));
			t_vec3	uv_minus_origin = sub_vec3(uv, origin);
			t_ray	r = new_ray(origin, add_vec3(lower_left_corner,uv_minus_origin));
			int x = i;
			t_color	pixel_color = ray_color(r);
			put_pixel(e.screen->img, x,  y, to_mlxcolor(pixel_color));
		}

	}
	printf("\ndone\n");
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);

	mlx_loop(e.mlx_ptr);
	return (0);
}
