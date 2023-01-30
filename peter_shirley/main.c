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
	//t_vec3 unit_dir = unit_vec3(r->direction);
	//double t = 0.5* (unit_dir.y + 1.0);
	// (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0)
	//t_vec3 background_color = (add_vec3(scalar_mul_vec3((1.0 - t) , new_vec3(1, 1, 1)) , scalar_mul_vec3(t , new_vec3(0.5, 0.7, 1.0))));
	t_vec3	background_color = new_color(0, 0, 0);

	if (depth <= 0)
		return (new_color(0, 0, 0));
	if (!hit(world, r, 0.001, INFINITY, &rec))
		return (background_color);
	t_color	emitted = material_emitted(rec.mat_ptr, rec.u, rec.v, &rec.p);
	t_ray	scattered;
	t_color	attenuation;
	if (!scatter(rec.mat_ptr, r, &rec, &attenuation, &scattered))
		return (emitted);
	return (add_vec3(emitted, mul_vec3(attenuation, ray_color(&scattered, world, depth - 1))));
}

double	random_double(void)
{
	return (rand() / (RAND_MAX + 1.0));
}

double	random_double_range(double min, double max)
{
	return (min + ((max - min) * random_double()));
}

int	random_int_range(int min, int max)
{
	return ( min + (rand() % (max - min + 1)));
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
							40,
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
	//hittable_list_add(world, sphere5);
	hittable_list_add(world, sphere6);
	hittable_list_add(world, sphere7);
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

void	setup_world5(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(13, 2, 3);
	t_point	lookat= new_point(0, 0, 0);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 10.0;
	const double	aperture = 0.0;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							20,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	t_texture	*noise = alloc_noise_texture();

	hittable_list_add(world, sphere_alloc(new_point(0, -1000, 0), 1000, alloc_lambertian(noise)));//alloc_solid_color(0.8, 0.8, 0.0))));
	hittable_list_add(world, sphere_alloc(new_point(0, 2, 0), 2, alloc_lambertian(noise)));
}

void	setup_world6(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(13, 2, 3);
	t_point	lookat= new_point(0, 1, 0);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 10.0;
	const double	aperture = 0.0;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							60,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	// geometries
	t_texture	*pertext;
	t_material	*diff_light;
	t_sphere	*sphere1;
	t_sphere	*sphere2;
	t_xy_rect	*rect1;
	t_xy_rect	*rect2;

	pertext = alloc_noise_texture();
	sphere1 = sphere_alloc(new_point(0, -1000, 0), 1000, alloc_lambertian(pertext));
	sphere2 = sphere_alloc(new_point(0, 2, 0), 2, alloc_lambertian(pertext));

	diff_light = alloc_diffuse_light(alloc_solid_color(4, 4, 4));
	rect1 = xyrect_alloc(3, 5, 1, 3, -2, diff_light);
	rect2 = xzrect_alloc(-1, 1, 2, 3, 5, diff_light);

	hittable_list_add(world, sphere1);
	hittable_list_add(world, sphere2);
	hittable_list_add(world, rect1);
	hittable_list_add(world, rect2);
}

// cornel_box
t_hittable_list	cornel_box(void)
{
	t_hittable_list	objects = {};

	t_material	*red = alloc_lambertian(alloc_solid_color(.65, .05, .05));
	t_material	*white = alloc_lambertian(alloc_solid_color(.73, .73, .73));
	t_material	*green = alloc_lambertian(alloc_solid_color(.12, .45, .15));
	t_material	*light = alloc_diffuse_light(alloc_solid_color(15, 15, 15));

	hittable_list_add(&objects, yzrect_alloc(0, 555, 0, 555, 555, green));
	hittable_list_add(&objects, yzrect_alloc(0, 555, 0, 555, 0, red));
	hittable_list_add(&objects, xzrect_alloc(213, 343, 227, 332, 554, light));
	hittable_list_add(&objects, xzrect_alloc(0, 555, 0, 555, 0, white));
	hittable_list_add(&objects, xzrect_alloc(0, 555, 0, 555, 555, white));
	hittable_list_add(&objects, xyrect_alloc(0, 555, 0, 555, 555, white));

	t_vec3	p0, p1, offset;
	t_hittable	*box1, *box2;

	// box1
	p0 = new_point(0, 0, 0);
	p1 = new_point(165, 330, 165);
	box1 = box_alloc(&p0, &p1, white);
	box1 = rotate_y_alloc(box1, 15);
	offset = new_vec3(265, 0, 295);
	box1 = translate_alloc(box1, &offset);
	hittable_list_add(&objects, box1);

	// box2
	p0 = new_point(0, 0, 0);
	p1 = new_point(165, 165, 165);
	box2 = box_alloc(&p0, &p1, white);
	box2 = rotate_y_alloc(box2, -18);
	offset = new_vec3(130, 0, 65);
	box2 = translate_alloc(box2, &offset);
	hittable_list_add(&objects, box2);
	return (objects);
}

void	setup_world7(t_camera *camera, t_hittable_list *world)
{
	// camera
	t_point	lookfrom = new_point(278, 278, -800);
	t_point	lookat= new_point(278, 278, 0);
	t_vec3	vup = new_vec3(0, 1, 0);
	double	dist_to_focus = 10.0;
	const double	aperture = 0.0;
	const double	vfov = 40.0;
	*camera = new_camera_default(lookfrom,
							lookat,
							vup,
							vfov,
							ASPECT_RATIO, 
							aperture,
							dist_to_focus);

	// geometries
	*world = cornel_box();
	/*
	t_material	*green = alloc_lambertian(alloc_solid_color(.12, .45, .15));
	hittable_list_add(world, sphere_alloc(new_point(200, 200, 200), 100, green));
	hittable_list_add_list(world, cornel_box());
	*/
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
	setup_world7(&camera, &world);

	world = new_bvh_node(world.next, NULL);
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
