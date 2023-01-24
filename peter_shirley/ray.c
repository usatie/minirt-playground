#include "ray.h"
#include "vec3.h"

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


t_ray	new_ray(t_point p, t_vec3 dir)
{
	t_ray	ret;

	ret.origin = p;
	ret.direction = dir;
	return (ret);
}

t_point	ray_at(t_ray ray, double t)
{
	t_point	at;

	at = add_vec3(ray.origin, scalar_mul_vec3(t, ray.direction));
	return (at);
}


double	hit_sphere(t_point	center, double radius, t_ray r)
{
	t_vec3	oc = sub_vec3(r.origin, center);
	double a  =length_squared_vec3(r.direction);
	double	half_b = dot_vec3(oc, r.direction);
	double c = length_squared_vec3(oc) - radius * radius;
	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (-1.0);
	return ((-half_b - sqrt(discriminant)) / a);

}


t_color	ray_color(t_ray r)
{
	t_vec3 unit_dir = unit_vec3(r.direction);
	double	t = hit_sphere(new_point(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		// N = N = unit_vector(r.at(t) - vec3(0,0,-1))
		t_vec3 N = unit_vec3(sub_vec3(ray_at(r, t), new_point(0, 0, -1)));
		return (scalar_mul_vec3(0.5, new_color(N.x + 1, N.y + 1, N.z + 1)));
	}
	t = 0.5* (unit_dir.y + 1.0);
	// (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0)
	return (add_vec3(scalar_mul_vec3((1.0 - t) , new_vec3(1, 1, 1)) , scalar_mul_vec3(t , new_vec3(0.5, 0.7, 1.0))));
}


