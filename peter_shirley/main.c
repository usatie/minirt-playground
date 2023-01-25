#include "rtweekend.h"
#include "color.h"
#include "mlx.h"

#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <unistd.h> // STDERR_FILENO
#include <stdio.h>  // dprintf

double	degrees_to_radians(double degrees)
{
	return (degrees * M_PI / 180);
}

t_color	ray_color(t_ray *r, const t_hittable_list *world)
{
	t_hit_record	rec;

	if (hit(world, r, 0, 10000000000, &rec))
	{
		// return 0.5 * (rec.normal + color(1,1,1));
		return (scalar_mul_vec3(0.5, add_vec3(rec.normal, new_color(1,1,1))));
	}
	t_vec3 unit_dir = unit_vec3(r->direction);
	double t = 0.5* (unit_dir.y + 1.0);
	// (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0)
	return (add_vec3(scalar_mul_vec3((1.0 - t) , new_vec3(1, 1, 1)) , scalar_mul_vec3(t , new_vec3(0.5, 0.7, 1.0))));
}

double	random_double(void)
{
	return (rand() / (RAND_MAX + 1.0));
}

double	random_double_range(double min, double max)
{
	return (min + ((max - min) * random_double()));
}

double	clamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

int	main(void)
{
	t_env		e;
	double		viewport_height = 2.0;
	double		viewport_width = ASPECT_RATIO * viewport_height;
	double		focal_length = 1.0;

	t_point		origin = new_point(0, 0, 0);
	t_vec3		horizontal = new_vec3(viewport_width, 0, 0);
	t_vec3		vertical = new_vec3(0, viewport_height, 0);
	t_vec3		mean_horizontal_vertical = scalar_div_vec3(add_vec3(horizontal, vertical), 2.0);
	// auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
	t_vec3		lower_left_corner = sub_vec3(sub_vec3(origin, mean_horizontal_vertical), new_vec3(0, 0, focal_length));
	t_camera	camera = new_camera(origin, horizontal, vertical);
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	t_hittable_list	world = {};
	t_sphere		sphere1 = sphere_new(new_vec3(0,0,-1), 0.5);
	t_sphere		sphere2 = sphere_new(new_vec3(0,-100.5,-1), 100);
	hittable_list_add(&world, &sphere1);
	hittable_list_add(&world, &sphere2);

	for (int j = WIN_HEIGHT - 1; j >=0;  --j)
	{
		int y = WIN_HEIGHT - j;
		dprintf(STDERR_FILENO, "\rscanlines remainings: %d", j);
		for (int i = 0; i < WIN_WIDTH; ++i)
		{
			double	u = (double)i / (WIN_WIDTH - 1);
			double	v = (double)j / (WIN_HEIGHT - 1);
			// ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
			t_ray	r = get_ray(&camera, u, v);
			int x = i;
			t_color	pixel_color = ray_color(&r, world.next);
			put_pixel(e.screen->img, x,  y, to_mlxcolor(pixel_color));
		}

	}
	printf("\ndone\n");
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);

	mlx_loop(e.mlx_ptr);
	return (0);
}
