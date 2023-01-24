#include "playground.h"

t_vec3	new_vec3(double x, double y, double z)
{
	t_vec3	ret;

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

double	dot_vec3(t_vec3 a, t_vec3 b)
{
	double	ret;

	ret = a.x * b.x + a.y * b.y + a.z * b.z;
	return (ret);
}

t_vec3	t_vec3_cross(t_vec3 *a, t_vec3 *b)
{
	t_vec3	ret;

	ret.x = a->y * b->z - a->z * b->y;
	ret.y = a->z * b->x - a->x * b->z;
	ret.z = a->x * b->y - a->y * b->x;
	return (ret);
}

double	mag_of_vec3(t_vec3 a)
{
	return (sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2)));
}

t_vec3	unit_vec3(t_vec3 a)
{
	double	norm;

	norm = mag_of_vec3(a);
	if (norm != 0)
		return (scalar_mul_vec3(1 / norm, a));
	return (new_vec3(0, 0, 0));
}


void	print_vec3(t_vec3 a)
{
	printf("vec3 [%lf %lf %lf]\n", a.x, a.y, a.z);
}