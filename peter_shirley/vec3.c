#include "vec3.h"
#include <stdio.h> // printf
#include "color.h" // t_rgb
#include "rtweekend.h" // clamp

t_vec3	new_vec3(double x, double y, double z)
{
	t_vec3	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_point	new_point(double x, double y, double z)
{
	t_point	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_color	new_color(double x, double y, double z)
{
	t_color	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_vec3	add_vec3(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;

	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return (ret);
}

t_vec3	sub_vec3(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;

	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return (ret);
}

t_vec3	mul_vec3(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;

	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return (ret);
}

t_vec3	scalar_mul_vec3(double t, t_vec3 a)
{
	t_vec3	ret;

	ret.x = a.x * t;
	ret.y = a.y * t;
	ret.z = a.z * t;
	return (ret);
}

t_vec3	scalar_div_vec3(t_vec3 a, double t)
{
	t_vec3	ret;

	ret.x = a.x / t;
	ret.y = a.y / t;
	ret.z = a.z / t;
	return (ret);
}

double	dot_vec3(t_vec3 a, t_vec3 b)
{
	double	ret;

	ret = a.x * b.x + a.y * b.y + a.z * b.z;
	return (ret);
}

t_vec3	cross_vec3(t_vec3 a, t_vec3 b)
{
	t_vec3	ret;

	ret.x = a.y * b.z - a.z * b.y;
	ret.y = a.z * b.x - a.x * b.z;
	ret.z = a.x * b.y - a.y * b.x;
	return (ret);
}

double	length_squared_vec3(t_vec3 a)
{
	return (pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}


double	length_vec3(t_vec3 a)
{
	return (sqrt(length_squared_vec3(a)));
}

t_vec3	unit_vec3(t_vec3 a)
{
	double	norm;

	norm = length_vec3(a);
	if (norm != 0)
		return (scalar_mul_vec3(1 / norm, a));
	return (new_vec3(0, 0, 0));
}

void	print_vec3(t_vec3 a)
{
	printf("vec3 [%lf %lf %lf]\n", a.x, a.y, a.z);
}

int		to_mlxcolor(t_color pixel_color, int samples_per_pixel)
{
	double	r = pixel_color.x;
	double	g = pixel_color.y;
	double	b = pixel_color.z;

	// Divide color sum by num samples
	double	scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	r = sqrt(r);
	g = sqrt(g);
	b = sqrt(b);
	t_rgb	rgbcol;
	rgbcol.rgb.r = (uint8_t)(256 * clamp(r, 0.0, 0.999));
	rgbcol.rgb.g = (uint8_t)(256 * clamp(g, 0.0, 0.999));
	rgbcol.rgb.b = (uint8_t)(256 * clamp(b, 0.0, 0.999));
	return (rgbcol.mlx_color);
}

t_vec3	random_vec3(void)
{
	t_vec3	ret;

	ret = new_vec3(random_double(), random_double(), random_double());
	return (ret);
}

t_vec3	random_range_vec3(double min, double max)
{
	t_vec3	ret;

	ret = new_vec3(random_double_range(min, max), random_double_range(min, max), random_double_range(min, max));
	return (ret);
}

t_vec3	random_in_unit_sphere()
{
	while(true)
	{
		t_vec3	ret;

		ret = random_range_vec3(-1, 1);
		if (length_squared_vec3(ret) >= 1)
			continue;
		return (ret);
	}
}
