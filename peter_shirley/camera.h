#ifndef CAMERA_H
# define CAMERA_H
# include "rtweekend.h"

typedef struct s_camera t_camera;

struct s_camera {
	double	aspect_ratio;
	double	viewport_height;
	double	viewport_width;
	double	lens_radius;
	t_point origin;
	t_point lower_left_corner;
	t_vec3 horizontal;
	t_vec3 vertical;
	t_vec3 u;
	t_vec3 v;
	t_vec3 w;
};

t_ray		get_ray(const t_camera *self, double u, double v);
t_camera	new_camera(t_point origin, t_vec3 horizontal, t_vec3 vertical, t_vec3 w, double aperture, double focus_dist);
t_camera	new_camera_default(t_point lookfrom, t_point lookat, t_vec3 vup,
								 double vfov, double aspect_ratio, double aperture, double focus_dist);

#endif
