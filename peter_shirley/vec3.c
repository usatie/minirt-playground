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

t_vec3	random_unit_vector()
{
	double	a = random_double_range(0, 2 * M_PI);
	double	z = random_double_range(-1, 1);
	double	r = sqrt(1 - z * z);
	return (new_vec3(r * cos(a), r * sin(a), z));
}

t_vec3	random_in_hemisphere(const t_vec3 *normal)
{
	t_vec3	in_unit_sphere = random_in_unit_sphere();
	if (dot_vec3(in_unit_sphere, *normal) > 0.0)
		return (in_unit_sphere);
	else
		return (scalar_mul_vec3(-1.0, in_unit_sphere));
}

t_vec3	random_in_unit_disk(void)
{
	while (1)
	{
		t_vec3 ret = new_vec3(random_double_range(-1, 1), random_double_range(-1, 1), 0);
		if (length_squared_vec3(ret) >= 1)
			continue;
		return (ret);
	}
}

t_vec3	reflect(const t_vec3 *v, const t_vec3 *n)
{
	double dot	= dot_vec3(*v, *n);
	return (sub_vec3(*v, scalar_mul_vec3(2 * dot, *n)));
}

t_vec3	refract(const t_vec3 *uv, const t_vec3 *n, double etai_over_etat)
{
	t_vec3	inverse_v = scalar_mul_vec3(-1.0, *uv);
	double	cos_thata = dot_vec3(inverse_v, *n);
	t_vec3	r_out_para = scalar_mul_vec3(etai_over_etat, add_vec3(*uv, scalar_mul_vec3(cos_thata, *n)));
	t_vec3	r_out_perp = scalar_mul_vec3(-1.0 * sqrt(1.0 - length_squared_vec3(r_out_para)), *n);
	return (add_vec3(r_out_para, r_out_perp));
}
