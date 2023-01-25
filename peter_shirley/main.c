#include "rtweekend.h"
#include "color.h"
#include "mlx.h"

#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <unistd.h> // STDERR_FILENO
#include <stdio.h>  // dprintf

double	degrees_to_radians(double degrees)
{
	return (degrees * M_PI / 180);
}

t_color	ray_color(t_ray *r, const t_hittable_list *world, int depth)
{
	t_hit_record	rec;

	if (depth <= 0)
		return (new_color(0, 0, 0));
	if (hit(world, r, 0.001, INFINITY, &rec))
	{
		t_ray	scattered;
		t_color	attenuation;
		
		if (scatter(rec.mat_ptr, r, &rec, &attenuation, &scattered))
			return (mul_vec3(attenuation, ray_color(&scattered, world, depth - 1)));
		return (new_color(0, 0, 0));
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

void	setup_world(t_hittable_list *world)
{
	t_material	*diff_mat1 = calloc(1, sizeof(t_material));
	t_material	*diff_mat2 = calloc(1, sizeof(t_material));
	t_material	*metal_mat1 = calloc(1, sizeof(t_material));
	//t_material	*metal_mat2 = calloc(1, sizeof(t_material));
	t_material	*dielec_mat = calloc(1, sizeof(t_material));

	*diff_mat2 = (t_material){LAMBERTIAN, new_color(0.1, 0.2, 0.5), 0, 0};
	*diff_mat1 = (t_material){LAMBERTIAN, new_color(0.8, 0.8, 0.0), 0, 0};
	*metal_mat1 = (t_material){METAL, new_color(0.8, 0.6, 0.2), 0, 0};
	//*metal_mat2 = (t_material){METAL, new_color(0.8, 0.8, 0.8), 0, 0};
	*dielec_mat = (t_material){DIELECTRIC, new_color(1, 1, 1), 0, 1.5};
	
	t_sphere		*sphere1 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere2 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere3 = calloc(1, sizeof(t_sphere));
	//t_sphere		*sphere4 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere5 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere6 = calloc(1, sizeof(t_sphere));

	*sphere1 = sphere_new(new_vec3(0,-100.5,-1), 100, diff_mat1);
	*sphere2 = sphere_new(new_vec3(0,0,-1), 0.5, diff_mat2);
	*sphere3 = sphere_new(new_vec3(1,0,-1), 0.5, metal_mat1);
	//*sphere4 = sphere_new(new_vec3(-1,0,-1), 0.5, metal_mat2);
	*sphere5 = sphere_new(new_vec3(-1,0,-1), 0.5, dielec_mat);
	*sphere6 = sphere_new(new_vec3(-1,0,-1), -0.45, dielec_mat);
	hittable_list_add(world, sphere1);
	hittable_list_add(world, sphere2);
	hittable_list_add(world, sphere3);
	// hittable_list_add(world, &sphere4);
	hittable_list_add(world, sphere5);
	hittable_list_add(world, sphere6);
}


void	setup_world2(t_hittable_list *world)
{
	double R = cos(M_PI_4);
	t_material	*diff_mat1 = calloc(1, sizeof(t_material));
	t_material	*diff_mat2 = calloc(1, sizeof(t_material));

	*diff_mat2 = (t_material){LAMBERTIAN, new_color(0, 0, 1), 0, 0};
	*diff_mat1 = (t_material){LAMBERTIAN, new_color(1, 0, 0), 0, 0};
	
	t_sphere		*sphere1 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere2 = calloc(1, sizeof(t_sphere));

	*sphere1 = sphere_new(new_vec3(R,0,-1), R, diff_mat1);
	*sphere2 = sphere_new(new_vec3(-R,0,-1), R, diff_mat2);
	hittable_list_add(world, sphere1);
	hittable_list_add(world, sphere2);
}

int	main(void)
{
	t_env		e;
	const int	samples_per_pixel = 100;
	const int	max_depth = 50;
	t_camera	camera = new_camera_default(new_point(-2, 2, 1), new_point(0, 0, -1), new_vec3(0, 1, 0), 20, ASPECT_RATIO);
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	t_hittable_list	world = {};

	setup_world(&world);

	for (int j = WIN_HEIGHT - 1; j >=0;  --j)
	{
		int y = WIN_HEIGHT - j;
		dprintf(STDERR_FILENO, "\rscanlines remainings: %d", j);
		for (int i = 0; i < WIN_WIDTH; ++i)
		{
			int x = i;
			t_color	pixel_color = new_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++)
			{
				double	u = ((double)i + random_double()) / (WIN_WIDTH - 1);
				double	v = ((double)j + random_double()) / (WIN_HEIGHT - 1);
				t_ray	r = get_ray(&camera, u, v);
				pixel_color = add_vec3(pixel_color, ray_color(&r, world.next, max_depth));

			}
			put_pixel(e.screen->img, x,  y, to_mlxcolor(pixel_color, samples_per_pixel));
		}
		mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
			e.screen->img->ptr, 0, 0);
	}
	printf("\ndone\n");
	mlx_loop(e.mlx_ptr);
	return (0);
}
