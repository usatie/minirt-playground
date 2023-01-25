#ifndef CAMERA_H
# define CAMERA_H
# include "rtweekend.h"

typedef struct s_camera t_camera;

struct s_camera {
	double	aspect_ratio;
	double	viewport_height;
	double	viewport_width;
	double	focal_length;
	t_point origin;
	t_point horizontal;
	t_point vertical;
	t_point lower_left_corner;
};

t_ray		get_ray(const t_camera *self, double u, double v);
t_camera	new_camera(t_point origin, t_vec3 horizontal, t_vec3 vertical);
t_camera	new_camera_default(void);

#endif
