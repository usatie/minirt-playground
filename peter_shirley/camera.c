#include "camera.h"
#include "rtweekend.h"
#include "ray.h"

t_camera	new_camera(t_point origin, t_vec3 horizontal, t_vec3 vertical, t_vec3 w, double aperture, double focus_dist)
{
	t_camera	c;

	c.aspect_ratio = ASPECT_RATIO;
	c.viewport_height = 2.0;
	c.viewport_width = c.viewport_height * c.aspect_ratio;
	c.lens_radius = aperture / 2;

	c.origin = origin;
	c.horizontal = horizontal;
	c.vertical = vertical;

	t_vec3	mean_horizontal_vertical = scalar_div_vec3(add_vec3(horizontal, vertical), 2.0);
	// auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
	c.lower_left_corner = sub_vec3(sub_vec3(origin, mean_horizontal_vertical), scalar_mul_vec3(focus_dist, w));
	return (c);
}

t_camera	new_camera_default(t_point lookfrom, t_point lookat, t_vec3 vup,
								 double vfov, double aspect_ratio, double aperture, double focus_dist)
{
	double		theta = degrees_to_radians(vfov);
	double		h = tan(theta / 2);
	double		viewport_height = 2.0 * h;
	double		viewport_width = aspect_ratio * viewport_height;

	t_vec3	w = unit_vec3(sub_vec3(lookfrom, lookat));
	t_vec3	u = unit_vec3(cross_vec3(vup, w));
	t_vec3	v = cross_vec3(w, u);
	
	t_point		origin = lookfrom;
	t_vec3		horizontal = scalar_mul_vec3(viewport_width * focus_dist, u);
	t_vec3		vertical = scalar_mul_vec3(viewport_height * focus_dist, v);
	t_camera	camera = new_camera(origin, horizontal, vertical, w, aperture, focus_dist);
	camera.u = u;
	camera.v = v;
	camera.w = w;
	

	return (camera);
}

t_ray	get_ray(const t_camera *self, double u, double v)
{
	t_vec3	rd =  scalar_mul_vec3(self->lens_radius, random_in_unit_disk());
	t_vec3	offset = add_vec3(scalar_mul_vec3(rd.x, self->u), scalar_mul_vec3(rd.y, self->v));
	// ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	t_vec3	uv = add_vec3(scalar_mul_vec3(u, self->horizontal), scalar_mul_vec3(v, self->vertical));
	t_vec3	uv_minus_origin_minus_offset = sub_vec3(sub_vec3(uv, self->origin), offset);
	return (new_ray(add_vec3(self->origin, offset), add_vec3(self->lower_left_corner,uv_minus_origin_minus_offset)));
}

