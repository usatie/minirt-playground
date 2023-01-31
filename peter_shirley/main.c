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
t_hittable_list	cornel_box1(void)
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

t_hittable_list	cornel_box2(void)
{
	t_hittable_list	objects = {};

	t_material	*red = alloc_lambertian(alloc_solid_color(.65, .05, .05));
	t_material	*white = alloc_lambertian(alloc_solid_color(.73, .73, .73));
	t_material	*green = alloc_lambertian(alloc_solid_color(.12, .45, .15));
	t_material	*light = alloc_diffuse_light(alloc_solid_color(7, 7, 7));

	hittable_list_add(&objects, yzrect_alloc(0, 555, 0, 555, 555, green));
	hittable_list_add(&objects, yzrect_alloc(0, 555, 0, 555, 0, red));
	hittable_list_add(&objects, xzrect_alloc(113, 443, 127, 432, 554, light));
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
	hittable_list_add(&objects, const_medium_alloc(box1, 0.01, alloc_solid_color(0, 0, 0)));

	// box2
	p0 = new_point(0, 0, 0);
	p1 = new_point(165, 165, 165);
	box2 = box_alloc(&p0, &p1, white);
	box2 = rotate_y_alloc(box2, -18);
	offset = new_vec3(130, 0, 65);
	box2 = translate_alloc(box2, &offset);
	hittable_list_add(&objects, const_medium_alloc(box2, 0.01, alloc_solid_color(1, 1, 1)));
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
	*world = cornel_box2();
	/*
	t_material	*green = alloc_lambertian(alloc_solid_color(.12, .45, .15));
	hittable_list_add(world, sphere_alloc(new_point(200, 200, 200), 100, green));
	hittable_list_add_list(world, cornel_box());
	*/
}

void	setup_world8(t_camera *camera, t_hittable_list *world)
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
	/*
	t_hittable_list	boxes1 = {};
	t_material		*ground = alloc_lambertian(alloc_solid_color(0.48, 0.83, 0.53));
	const int	boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			double	w = 100.0;
			double	x0 = -1000.0 + i*w;
			double	z0 = -1000.0 + j*w;
			double	y0 = 0.0;
			double	x1 = x0 + w;
			double	y1 = random_double_range(1, 101);
			double	z1 = z0 + w;
			t_vec3	p0 = new_point(x0, y0, z0);
			t_vec3	p1 = new_point(x1, y1, z1);
			hittable_list_add(&boxes1, box_alloc(&p0, &p1, ground));
		}
	}

	//t_hittable_list	*objects = hittable_list_alloc();

	// ground boxes
	hittable_list_add(world, alloc_bvh_node(boxes1.next, NULL));
	*/
	// light
	t_material	*light = alloc_diffuse_light(alloc_solid_color(7, 7, 7));
	hittable_list_add(world, xzrect_alloc(123, 423, 147, 412, 554, light));
	/**/
	// moving sphere
	t_point	center1 = new_point(400, 400, 200);
	t_point	center2 = add_vec3(center1, new_vec3(30, 0, 0));
	t_material	*moving_sphere_material = alloc_lambertian(alloc_solid_color(0.7, 0.3, 0.1));
	hittable_list_add(world, sphere_alloc(center2, 50, moving_sphere_material));

	// glass sphere
	hittable_list_add(world, sphere_alloc(new_point(260, 150, 45), 50, alloc_dielectric(1.5)));
	
	// metal sphere
	hittable_list_add(world, sphere_alloc(new_point(0, 150, 145), 50, alloc_metal(alloc_solid_color(0.8, 0.8, 0.9), 10.0)));
	
	// Blue smoke in glass sphere
	t_sphere	*boundary = sphere_alloc(new_point(360, 150, 145), 70, alloc_dielectric(1.5));
	t_const_medium *const_med = const_medium_alloc(boundary, 0.2, alloc_solid_color(0.2, 0.4, 0.9));
	printf("boundary = %p, boundary->boundary = %p\n", boundary, boundary->boundary);
	printf("const_med = %p, const_med->boundary = %p\n", const_med, const_med->boundary);
	//hittable_list_add(world, boundary);
	hittable_list_add(world, const_med);
	for (t_hittable *obj = world->next; obj; obj = obj->next) {
		printf("obj = %p, obj->type = %d obj->pre = %p\n", obj, obj->type, obj->pre);
	}
	/*
	// White smoke in the room
	boundary = sphere_alloc(new_point(0, 0, 0), 5000, alloc_dielectric(1.5));
	hittable_list_add(world, const_medium_alloc(boundary, 0.0001, alloc_solid_color(1, 1, 1)));

	// earth sphere
	t_material	*emat = alloc_lambertian(alloc_solid_color(0, 0, 1));
	hittable_list_add(world, sphere_alloc(new_point(400, 200, 400), 100, emat));

	// noise sphere
	t_texture	*pertext = alloc_noise_texture();
	hittable_list_add(world, sphere_alloc(new_point(220, 280, 300), 80, alloc_lambertian(pertext)));

	// takagi
    t_hittable_list    *boxes2 = hittable_list_alloc();
    t_material    *white = alloc_lambertian(alloc_solid_color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        hittable_list_add(boxes2, sphere_alloc(random_range_vec3(0, 165), 10, white));
    }
	t_vec3	offset = new_vec3(-100, 270, 395);
	hittable_list_add(
		world,
		translate_alloc(rotate_y_alloc(
			alloc_bvh_node(boxes2, NULL), 15), 
			&offset
		)
	);
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
	setup_world8(&camera, &world);
	for (t_hittable *h = world.next; h; h = h->next) {
		t_aabb	box;
		bounding_box(h, &box);
		printf("before obj %p, pre %p, next %p, h.x = %f\n", h, h->pre, h->next, box.min.x);
	}
	t_hittable *end;
	for (t_hittable *h = world.next; h; h = h->next) {
		end = h;
	}
	sort_hittable_list(world.next, end, box_x_compare);
	for (t_hittable *h = world.next; h; h = h->next) {
		t_aabb	box;
		bounding_box(h, &box);
		printf("obj %p, pre %p, next %p, h.x = %f\n", h, h->pre, h->next, box.min.x);
	}
	exit(1);
	// これ AND (青い煙 and 青い煙の周りのガラス球)が組み合わさるとSegmentation Faultする
	printf("Hello1\n");
	world = new_bvh_node(world.next, end);
	printf("Hello2\n");
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
