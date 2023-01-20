#ifndef PLAYGROUND_H
# define PLAYGROUND_H

# define WIN_WIDTH 400
# define WIN_HEIGHT 400

#include "color.h"

typedef struct point	point;
typedef struct pvector	pvector;
typedef struct s_img	t_img;
typedef struct s_screen	t_screen;
typedef struct s_env	t_env;
typedef struct s_ray	t_ray;
typedef struct s_shape	t_shape;
typedef struct s_shape	t_sphere;
typedef struct pvector t_fcolor;
typedef enum e_shape_kind t_shape_kind;
typedef struct s_intersection_point t_intersection_point;

struct point {
	int	x;
	int	y;
};

struct pvector {
	float	x;
	float	y;
	float	z;
};

struct s_img {
	void	*mlx_ptr;
	void	*ptr;
	char	*data;
	int		bits_per_pixel;
	int		bytes_per_pixel;
	int		bytes_per_line;
	int		endian;
};

struct s_screen {
	void			*win_ptr;
	t_img			*img;
};

// struct s_scene {
// 	float	ambient_intesity;
// 	float	light_intensity
// }

enum e_shape_kind {
	SPHERE,
	PLANE,
	CYLINDER,
};

struct s_shape {
	t_shape_kind	kind;
	t_rgb			color;
	//sphere
	pvector			*center;
	float			radius;
	//plane
	pvector			*normal;
	pvector			*position;

};

struct s_intersection_point {
	float	distance;
	pvector	*position;
	pvector	*normal;
};

t_intersection_point	*test_intersection(t_shape *shape, t_ray *ray);

struct s_light_source {
	pvector 	*position;
	t_fcolor	intencity;
	float		distance;
};

struct s_env {
	void		*mlx_ptr;
	t_screen	*screen;
};

/*
struct s_sphere {
	pvector *center;
	float	diameter;
	float	k_amb;
};
*/

struct s_ray {
	pvector	*start;
	pvector	*direction;
};

float constrain(float v, float vmin, float vmax); // 値の制限
float map(float v, float vmin, float vmax, float tmin, float tmax); // 値のマッピング
pvector	*pvector_new(float x, float y, float z);
void	pvector_set(pvector *v, float x, float y, float z);
pvector	*pvector_copy(pvector *v);
pvector	*pvector_add(pvector *v1, pvector *v2);
pvector	*pvector_sub(pvector *v1, pvector *v2);
pvector	*pvector_mul(pvector *v, float k);
float	pvector_dot(pvector *v1, pvector *v2);
float	pvector_magsq(pvector *v);
float	pvector_mag(pvector *v);
void	pvector_normalize(pvector *v);
void	pvector_print(pvector *v);

t_screen	*init_screen(void *mlx_ptr);
void	*init_img(void *mlx_ptr, int width, int height);
void	put_pixel(const t_img *img, int x, int y, int mlx_color);
#endif
