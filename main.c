#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "mlx.h"

# define WIN_WIDTH 400
# define WIN_HEIGHT 400

void	*init_img(void *mlx_ptr, int width, int height)
{
	t_img	*img;

	img = malloc(sizeof(t_img));
	if (img == NULL)
		return (NULL);
	img->ptr = mlx_new_image(mlx_ptr, width, height);
	if (img->ptr == NULL)
	{
		free(img);
		return (NULL);
	}
	img->mlx_ptr = mlx_ptr;
	img->data = mlx_get_data_addr(
			img->ptr,
			&img->bits_per_pixel,
			&img->bytes_per_line,
			&img->endian);
	img->bytes_per_pixel = img->bits_per_pixel / 8;
	return (img);
}

static char	get_n_th_byte(int x, int n)
{
	const int	size = 8;
	const int	index = n * size;
	int			mask;
	char		ret;

	mask = ((n << size) - 1) << index;
	ret = (char)((x & mask) >> index);
	return (ret);
}

// Update img data
// If endian is little endian, write from head,
// Else write from tail.
void	put_pixel(const t_img *img, int x, int y, int mlx_color)
{
	int	x_color;
	int	head;
	int	i;
	int	index;

	x_color = mlx_get_color_value(img->mlx_ptr, mlx_color);
	head = (x * img->bytes_per_pixel) + (y * img->bytes_per_line);
	i = 0;
	while (i < img->bytes_per_pixel)
	{
		if (img->endian == 0)
		{
			index = head + i;
		}
		else
		{
			index = head + img->bytes_per_pixel - 1 - i;
		}
		img->data[index] = get_n_th_byte(x_color, i);
		i++;
	}
}

t_screen	*init_screen(void *mlx_ptr)
{
	t_screen	*screen;

	screen = calloc(1, sizeof(*screen));
	screen->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "screen");
	screen->img = init_img(mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	return (screen);
}

typedef struct s_sphere	t_sphere;
typedef struct s_ray	t_ray;

struct s_sphere {
	pvector *center;
	float	diameter;
};

/*
typedef struct s_obj	t_obj;
struct s_obj {
	enum e_obj_kind	kind; // OBJ_SPHERE, OBJ_PLANE, OBJ_CYLINDER

	// SPHERE
	pvector *center;
	float	diameter;

	// PLANE
	pvector *n;
	float	start;

	// CYLINDER
	pvector *n;
	float	diameter;

	t_obj	*next;
};
*/

struct s_ray {
	pvector	*start;
	pvector	*direction;
};

t_sphere	*sphere_new(pvector *center, float diameter)
{
	t_sphere	*sp;

	sp = calloc(1, sizeof(*sp));
	sp->center = center;
	sp->diameter = diameter;
	return (sp);
}

t_ray	*ray_new(pvector *start, pvector *direction)
{
	t_ray	*ray;

	ray = calloc(1, sizeof(*ray));
	ray->start = start;
	ray->direction = direction;
	return (ray);
}

int	sphere_intersect(t_sphere *sp, t_ray *ray, float *t1, float *t2)
{
	pvector *d = ray->direction;
	pvector *s = ray->start;
	pvector *pc = sp->center;
	float	r = sp->diameter;
	float	a = pvector_dot(d, d);
	float	b = 2.0 * pvector_dot(pvector_sub(s, pc), d);
	float	c = pvector_magsq(pvector_sub(s, pc)) - r * r;
	float	D = b * b - 4.0 * a * c;

	if (D > 0)
	{
		*t1 = (-b - sqrt(D)) / (2.0 * a);
		*t2 = (-b + sqrt(D)) / (2.0 * a);
		return (2);
	}
	else if (D == 0)
	{
		*t1 = *t2 = (-b) / (2.0 * a);
		return (1);
	}
	else
		return (0);
}

#define BG_COLOR 0x64aaee

/*
int	main()
{
	l1 = ambient_light();
	l2 = diffuse_light();
	l3 = specular_light();
	light = l1 + l2 + l3;
}
*/

t_ray *get_ray(int x, int y, pvector *camera)
{
	pvector *x_dir = pvector_new(1, 0, 0);
	pvector *y_dir = pvector_new(0, -1, 0);
	float u = map(x, 0, WIN_WIDTH - 1, -1, 1);
	float v = map(y, 0, WIN_WIDTH - 1, 1, -1);

	pvector *ray_dir = pvector_sub(pvector_add(pvector_mul(x_dir, u), pvector_mul(y_dir, v)), camera);
	return (ray_new(camera, ray_dir));
}

int	ambient_light(int x, int y, t_ray *ray, t_sphere *sphere)
{
	float t1, t2;
	(void)x;
	(void)y;
	
	if (sphere_intersect(sphere, ray, &t1, &t2) > 0)
	{
		if (t1 > 0 || t2 > 0)
			return(0xff0000);
	}
	return (0);
}

float	diffuse_light(t_ray *ray, t_sphere *sphere, float t, pvector *light)
{
	pvector	*intersection;
	pvector	*n;
	pvector	*ray_light;

	intersection = pvector_add(ray->start, pvector_mul(ray->direction, t));
	n = pvector_sub(intersection, sphere->center);
	pvector_normalize(n);
	ray_light = pvector_sub(light, intersection);
	pvector_normalize(ray_light);
	return (constrain(pvector_dot(ray_light, n), 0 , 1));
	/*float t1, t2;
	(void)x;
	(void)y;
	
	if (sphere_intersect(sphere, ray, &t1, &t2) > 0)
	{
		if (t1 > 0 || t2 > 0)
			return(0xff0000);
	}
	return (0);*/
}

typedef struct s_color	t_color;
struct s_color {
	int	alpha;
	int	r;
	int	g;
	int b;
};

t_color	color_add(t_color col1, t_color col2)
{
	col1.r += col2.r;
	col1.g += col2.g;
	col1.b += col2.b;
	if (col1.r > 255)
		col1.r = 255;
	if (col1.g > 255)
		col1.g = 255;
	if (col1.b > 255)
		col1.b = 255;
	return (col1);
}


float	has_intersection(t_ray *ray, t_sphere *sp)
{
	pvector *d = ray->direction;
	pvector *s = ray->start;
	pvector *pc = sp->center;
	float	r = sp->diameter;
	float	a = pvector_dot(d, d);
	float	b = 2.0 * pvector_dot(pvector_sub(s, pc), d);
	float	c = pvector_magsq(pvector_sub(s, pc)) - r * r;
	float	D = b * b - 4.0 * a * c;

	if (D > 0)
	{
		float t1 = (-b - sqrt(D)) / (2.0 * a);
		float t2 = (-b + sqrt(D)) / (2.0 * a);
		if (t1 > 0)
			return (t1);
		return (t2);
	}
	else if (D == 0)
	{
		float t1 = (-b) / (2.0 * a);
		return (t1);
	}
	return (-1);}

int	main(void)
{
	t_env	e;
	
	e.mlx_ptr = mlx_init();
	e.screen = init_screen(e.mlx_ptr);
	pvector *camera;
	pvector *light;
	t_sphere	*sphere;

	sphere = sphere_new(pvector_new(0, 0, 5), 1.0);
	camera = pvector_new(0, 0, -5);
	light = pvector_new(-5, 5, -5);

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		for (int y = 0; y < WIN_HEIGHT; y++)
		{
			//bool	intersection;
			int		color = BG_COLOR;
			float t;
			t_ray *ray = get_ray(x, y, camera);
			t = has_intersection(ray, sphere);
			if (t > 0)
			{
				float	R = 0;
				//R += ambient_light(x, y, camera, sphere);
				R += diffuse_light(ray, sphere, t, light);
				//R += specular_light(x, y, camera, sphere);
				printf("R %f\n", R);
				color = 255 * R;
				printf("color %0x\n", color);
			}
			put_pixel(e.screen->img, x, y, color);
		}
	}
	mlx_put_image_to_window(e.mlx_ptr, e.screen->win_ptr,
		e.screen->img->ptr, 0, 0);
	mlx_loop(e.mlx_ptr);
	return (0);
}

void	test_vector(void)
{
	printf("constrain(-1, 0, 10) = %f\n", constrain(-1, 0, 10));
	printf("map(5, 0, 10, 0, 1) = %f\n", map(5, 0, 10, 0, 1));

	pvector	*v1;
	pvector	*v2;

	v1 = pvector_new(1.0, 2.0, 3.0);
	v2 = pvector_copy(v1);
	printf("v1: ");
	pvector_print(v1);
	printf("\n");

	pvector_normalize(v2);

	printf("v2: ");
	pvector_print(v2);
	printf("\n");
}
