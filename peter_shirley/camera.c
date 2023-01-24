#include "camera.h"
#include "ray.h"

t_camera	new_camera(t_point origin, t_vec3 horizontal, t_vec3 vertical)
{
	t_camera	c;

	c.aspect_ratio = ASPECT_RATIO;
	c.viewport_height = 2.0;
	c.viewport_width = c.viewport_height * c.aspect_ratio;
	c.focal_length = 1.0;

	c.origin = origin;
	c.horizontal = horizontal;
	c.vertical = vertical;
	t_vec3	mean_horizontal_vertical = scalar_div_vec3(add_vec3(horizontal, vertical), 2.0);
	// auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
	c.lower_left_corner = sub_vec3(sub_vec3(origin, mean_horizontal_vertical), new_vec3(0, 0, c.focal_length));
	return (c);
}

t_ray	get_ray(const t_camera *self, double u, double v)
{
	// ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	t_vec3	uv = add_vec3(scalar_mul_vec3(u, self->horizontal), scalar_mul_vec3(v, self->vertical));
	t_vec3	uv_minus_origin = sub_vec3(uv, self->origin);
	return (new_ray(self->origin, add_vec3(self->lower_left_corner,uv_minus_origin)));
}