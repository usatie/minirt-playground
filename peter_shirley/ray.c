#include "ray.h"
#include "vec3.h"

t_ray	new_ray(t_point p, t_vec3 dir)
{
	t_ray	ret;

	ret.origin = p;
	ret.direction = dir;
	return (ret);
}

t_point	ray_at(const t_ray *ray, double t)
{
	t_point	at;

	at = add_vec3(ray->origin, scalar_mul_vec3(t, ray->direction));
	return (at);
}


double	hit_sphere(t_point	center, double radius, t_ray *r)
{
	t_vec3	oc = sub_vec3(r->origin, center);
	double a  =length_squared_vec3(r->direction);
	double	half_b = dot_vec3(oc, r->direction);
	double c = length_squared_vec3(oc) - radius * radius;
	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (-1.0);
	return ((-half_b - sqrt(discriminant)) / a);

}
