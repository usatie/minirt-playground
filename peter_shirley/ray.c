#include "playground.h"

t_point	new_point(double x, double y, double z)
{
	t_point	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_ray	new_ray(t_point p, t_vec3 dir)
{
	t_ray	ret;

	ret.origin = p;
	ret.direction = dir;
	return (ret);
}

t_point	ray_at(double t, t_ray ray)
{
	t_point	at;

	at = add_vec3(ray.origin, scalar_mul_vec3(t, ray.direction));
	return (at);
}

t_color	ray_color(t_ray ray)
{
	t_vec3 unit_dir = unit_vec3(ray.direction);
	double t = 0.5* (unit_dir.y + 1.0);
	return (add_vec3(scalar_mul_vec3((1.0 - t) , new_vec3(1, 1, 1)) , scalar_mul_vec3(t , new_vec3(0.5, 0.7, 1.0))));
}


