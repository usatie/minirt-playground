#ifndef PLAYGROUND_H
# define PLAYGROUND_H

# define WIN_WIDTH 512
# define WIN_HEIGHT 512

# include <stdbool.h>
# include "color.h"

typedef struct point	point;
typedef struct pvector	pvector;
typedef struct s_img	t_img;
typedef struct s_screen	t_screen;
typedef struct s_env	t_env;
typedef struct s_ray	t_ray;
typedef struct s_shape	t_shape;
typedef struct s_light_source	t_light_source;
typedef struct s_lighting	t_lighting;
typedef struct s_shape	t_sphere;
typedef struct s_shape	t_plane;
typedef enum   e_light_kind	t_light_kind;
typedef struct s_fcolor t_fcolor;
typedef enum   e_shape_kind t_shape_kind;
typedef struct s_intersection_point t_intersection_point;
typedef struct s_material t_material;
typedef struct s_scene	t_scene;
typedef struct s_intersection_test_result t_intersection_test_result;

struct s_scene {
	t_shape			*shapes;
	t_light_source	*light_sources;
	t_fcolor		*ambient_intesity;
	float			air_refraction;
};

struct point {
	int	x;
	int	y;
};

struct pvector {
	float	x;
	float	y;
	float	z;
};

struct s_fcolor {
	float	red;
	float	green;
	float	blue;
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

struct s_material {
	t_fcolor	*amibient_factor;
	t_fcolor	*diffuse_factor;
	t_fcolor	*specular_factor;
	float		shineness;
	bool		use_perfect_reflectance; // computing perfect reflectance is very costy
	t_fcolor	*catadioptric_factor;
	bool		use_refraction;
	float		refraction;
};

struct s_shape {
	t_shape_kind	kind;
	t_material		*material;
	t_shape			*next;

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

struct s_intersection_test_result {
	t_shape					*shape;
	t_intersection_point	*intersection_point;
};

enum e_light_kind {
	POINT,
	DIRECTIONAL,
};

struct s_light_source {
	t_light_kind	kind;
	t_fcolor		*intencity;
	t_light_source	*next;

	//point light
	pvector			*position;
	//directional light
	pvector			*direction;
};

struct s_lighting {
	pvector 	*direction;
	t_fcolor	*intencity;
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

// vector.c
float 	constrain(float v, float vmin, float vmax); // 値の制限
float 	map(float v, float vmin, float vmax, float tmin, float tmax); // 値のマッピング
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

// fcolor.c
t_fcolor	*fcolor_new(float red, float green, float blue);
void		fcolor_set(t_fcolor *c, float red, float green, float blue);
t_fcolor	*fcolor_copy(t_fcolor *c);
t_fcolor	*fcolor_add(t_fcolor *c1, t_fcolor *c2);
t_fcolor	*fcolor_mul(t_fcolor *c1, t_fcolor *c2);
t_rgb		fcolor2rgb(t_fcolor *fcolor);

// material.c
t_material	*material_new(void);

// shape.c
t_shape	*shape_new(t_shape_kind kind);

// ray.c
t_ray	*ray_new(pvector *start, pvector *direction);

// light_source.c
t_light_source	*light_source_new(t_light_kind kind);
t_lighting		*lighting_at(pvector *pos, t_light_source *light_source);

// scene.c
t_scene	*get_scene(void);
t_scene	*get_scene2(void);

// intersection.c
t_intersection_point		*test_intersection(t_shape *shape, t_ray *ray);
t_intersection_test_result	*test_intersection_with_all(t_scene *scene, t_ray *ray);
t_intersection_test_result	*test_intersection_with_all_overload(t_scene *scene, t_ray *ray, float max_dist, bool exit_once_found);

// reflect.c
t_fcolor	*ambient_light(t_ray *ray, t_shape *shape);
t_fcolor	*diffuse_light(t_shape *shape, t_intersection_point *intersection, t_lighting *lighting);
t_fcolor	*specular_light(t_ray *ray, t_shape *shape, t_intersection_point *intersection, t_lighting *lighting);

// ray_trace.c
t_fcolor	*ray_trace(t_scene *scene, t_ray *ray);
t_fcolor	*ray_trace_recursive(t_scene *scene, t_ray *ray, int recursion_level);

// mlx_utils.c
t_screen	*init_screen(void *mlx_ptr);
void		*init_img(void *mlx_ptr, int width, int height);
void		put_pixel(const t_img *img, int x, int y, int mlx_color);
#endif
