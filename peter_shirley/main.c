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

	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	for (int j = WIN_HEIGHT - 1; j >=0;  --j)
	{
		int y = WIN_HEIGHT - j;
		dprintf(STDERR_FILENO, "\rscanlines remainings: %d", j);
		for (int i = 0; i < WIN_WIDTH; ++i)
		{
			int x = i;
			t_rgb	color;
			double r = (double)i / (WIN_WIDTH - 1);
			double g = (double)j / (WIN_HEIGHT - 1);
			double b = 0.25;
			color.rgb.r = (int)(r * 255.999);
			color.rgb.g = (int)(g * 255.999);
			color.rgb.b = (int)(b * 255.999);
			put_pixel(e.screen->img, x,  y, color.mlx_color);
		}

	}
	printf("\ndone\n");
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);

	mlx_loop(e.mlx_ptr);
	return (0);
}
