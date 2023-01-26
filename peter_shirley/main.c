#include "rtweekend.h"
#include "color.h"
#include "mlx.h"

#include "camera.h"
#include "material.h"

#include <unistd.h> // STDERR_FILENO
#include <stdio.h>  // dprintf
#include <stdlib.h> // RAND_MAX, rand()

double	degrees_to_radians(double degrees)
{
	return (degrees * M_PI / 180);
}

t_color	ray_color(t_ray *r, const t_hittable_list *world, int depth)
{
	t_hit_record	rec;

	if (depth <= 0)
		return (new_color(0, 0, 0));
	if (!hit(world, r, 0.001, INFINITY, &rec))
		return (new_color(0, 0, 0));

	t_color	emitted = material_emitted(rec.mat_ptr, rec.u, rec.v, &rec.p);
	t_ray	scattered;
	t_color	attenuation;
	
	if (!scatter(rec.mat_ptr, r, &rec, &attenuation, &scattered))
		return (emitted);
	return (add_vec3(emitted, mul_vec3(attenuation, ray_color(&scattered, world, depth - 1))));
	// t_vec3 unit_dir = unit_vec3(r->direction);
	// double t = 0.5* (unit_dir.y + 1.0);
	// // (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0)
	// return (add_vec3(scalar_mul_vec3((1.0 - t) , new_vec3(1, 1, 1)) , scalar_mul_vec3(t , new_vec3(0.5, 0.7, 1.0))));
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

void	setup_world(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(-2, 2, 3);
	t_point	lookat= new_point(0, 0, 0);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 5.0;
	const double	aperture = 0.05;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							80,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	// geometries
	t_material	*diff_mat1;
	t_material	*diff_mat2;
	t_material	*metal_mat1;
	t_material	*dielec_mat;
	t_material	*mixed_mat;
	t_material	*diffuse_light_mat;

	diff_mat2 = alloc_lambertian(alloc_solid_color(0.1, 0.2, 0.5));
	diff_mat1 = alloc_lambertian(alloc_solid_color(0.8, 0.8, 0.0));
	metal_mat1 = alloc_metal(alloc_solid_color(0.8, 0.6, 0.2), 0);
	dielec_mat = alloc_dielectric(1.5);
	mixed_mat = alloc_mixed_material(alloc_solid_color(0.1, 0.2, 0.5), 0, 0.7);
	diffuse_light_mat = alloc_diffuse_light(alloc_solid_color(20, 20, 20));
	
	t_sphere		*sphere1 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere2 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere3 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere4 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere5 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere6 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere7 = calloc(1, sizeof(t_sphere));
	t_xy_rect		*rect = xyrect_alloc(-1, 1, 2, 3, -2, diffuse_light_mat);

	*sphere1 = sphere_new(new_vec3(0,-100.5,-1), 100, diff_mat1);
	*sphere2 = sphere_new(new_vec3(0,0,-1), 0.5, diff_mat2);
	*sphere3 = sphere_new(new_vec3(2,0,-1), 0.5, metal_mat1);
	*sphere4 = sphere_new(new_vec3(-1,0,-1), 0.5, dielec_mat);
	*sphere5 = sphere_new(new_vec3(-1,0,-1), -0.45, dielec_mat);
	*sphere6 = sphere_new(new_vec3(1,0,-1), 0.5, mixed_mat);
	*sphere7 = sphere_new(new_vec3(1, 1,-1), 0.5, diffuse_light_mat);
	hittable_list_add(world, sphere1);
	hittable_list_add(world, sphere2);
	hittable_list_add(world, sphere3);
	hittable_list_add(world, sphere4);
	hittable_list_add(world, sphere5);
	hittable_list_add(world, sphere6);
	//hittable_list_add(world, sphere7);
	hittable_list_add(world, rect);
}


void	setup_world2(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(0, 0, 0);
	t_point	lookat= new_point(0, 0, -1);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 1.0;
	const double	aperture = 0.05;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							80,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	// geometries
	double R = cos(M_PI_4);
	t_material	*diff_mat1;
	t_material	*diff_mat2;

	diff_mat2 = alloc_lambertian(alloc_solid_color(0, 0, 1));
	diff_mat1 = alloc_lambertian(alloc_solid_color(1, 0, 0));
	
	t_sphere		*sphere1 = calloc(1, sizeof(t_sphere));
	t_sphere		*sphere2 = calloc(1, sizeof(t_sphere));

	*sphere1 = sphere_new(new_vec3(R,0,-1), R, diff_mat1);
	*sphere2 = sphere_new(new_vec3(-R,0,-1), R, diff_mat2);
	hittable_list_add(world, sphere1);
	hittable_list_add(world, sphere2);
}

void	setup_world3(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(13, 2, 3);
	t_point	lookat= new_point(0, 0, 0);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 10.0;
	const double	aperture = 0.05;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							20,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	t_texture	*checker = alloc_checker_texture(
			alloc_solid_color(0.2, 0.3, 0.1),
			alloc_solid_color(0.9, 0.9, 0.9)
			);
	//t_material	*ground_material = alloc_lambertian(alloc_solid_color(0.5, 0.5, 0.5));
	t_material	*ground_material = alloc_lambertian(checker);
	hittable_list_add(world, sphere_alloc(new_point(0, -1000, 0), 1000, ground_material));
	for (int a = -8; a < 8; a++) {
		for (int b = -8; b < 8; b++) {
			double	choose_mat = random_double();
			t_point	center = new_point(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			t_point	fixed_point = new_point(4, 0.2, 0);
			if (length_vec3(sub_vec3(center, fixed_point)) > 0.9) {
				t_material	*sphere_material;
				if (choose_mat < 0.8) {
					// diffuse
					t_color	solid_color = mul_vec3(random_color(), random_color());
					t_solid_color	*albedo = alloc_solid_color(solid_color.x, solid_color.y, solid_color.z);
					sphere_material = alloc_lambertian(albedo);
					hittable_list_add(world, sphere_alloc(center, 0.2, sphere_material));
				} else if (choose_mat < 0.95) {
					// metal
					t_color	solid_color = new_color(random_double_range(0.5, 1), random_double_range(0.5, 1), random_double_range(0.5, 1));
					double	fuzz = random_double_range(0, 0.5);
					t_solid_color	*albedo = alloc_solid_color(solid_color.x, solid_color.y, solid_color.z);
					sphere_material = alloc_metal(albedo, fuzz);
					hittable_list_add(world, sphere_alloc(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = alloc_dielectric(1.5);
					hittable_list_add(world, sphere_alloc(center, 0.2, sphere_material));
				}
			}
		}
	}
	t_material	*material1 = alloc_dielectric(1.5);
	hittable_list_add(world, sphere_alloc(new_point(0, 1, 0), 1.0, material1));
	t_material	*material2 = alloc_lambertian(alloc_solid_color(0.4, 0.2, 0.1));
	hittable_list_add(world, sphere_alloc(new_point(-4, 1, 0), 1.0, material2));
	t_material	*material3 = alloc_metal(alloc_solid_color(0.7, 0.6, 0.5), 0);
	hittable_list_add(world, sphere_alloc(new_point(4, 1, 0), 1.0, material3));
}

void	setup_world4(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(13, 2, 3);
	t_point	lookat= new_point(0, 0, 0);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 10.0;
	const double	aperture = 0.05;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							20,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	t_texture	*checker = alloc_checker_texture(
			alloc_solid_color(0.2, 0.3, 0.1),
			alloc_solid_color(0.9, 0.9, 0.9)
			);

	hittable_list_add(world, sphere_alloc(new_point(0, -10, 0), 10, alloc_lambertian(checker)));
	hittable_list_add(world, sphere_alloc(new_point(0, 10, 0), 10, alloc_lambertian(checker)));
}

int	main(void)
{
	t_env		e;
	const int	samples_per_pixel = 100;
	const int	max_depth = 50;
	
	t_camera	camera;
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	t_hittable_list	world = {};

	world.type = HITTABLE_LIST;
	setup_world(&camera, &world);

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
				pixel_color = add_vec3(pixel_color, ray_color(&r, &world, max_depth));

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
