#ifndef PLAYGROUND_H
# define PLAYGROUND_H

# define WIN_WIDTH 256
# define WIN_HEIGHT 256

# include <stdbool.h>
# include <stdio.h>
# include <math.h>
# include "mlx_utils.h"
# include "color.h"

typedef struct s_env	t_env;
typedef struct s_vec3	t_vec3;
typedef struct s_vec3	t_point;
typedef struct s_vec3	t_color;
typedef struct s_ray	t_ray;

struct s_env {
	void		*mlx_ptr;
	t_screen	*screen;
};

struct s_vec3 {
	double	x;
	double	y;
	double	z;
};

struct s_ray {
	t_point	origin;
	t_vec3	direction;
};

//vec3.c
t_vec3	new_vec3(double x, double y, double z);
t_vec3	add_vec3(t_vec3 a, t_vec3 b);
t_vec3	sub_vec3(t_vec3 a, t_vec3 b);
t_vec3	mul_vec3(t_vec3 a, t_vec3 b);
t_vec3	scalar_mul_vec3(double t, t_vec3 a);
double	dot_vec3(t_vec3 a, t_vec3 b);
t_vec3	cross_vec3(t_vec3 a, t_vec3 b);
double	length_squared_vec3(t_vec3 a);
double	length_vec3(t_vec3 a);
t_vec3	unit_vec3(t_vec3 a);
void	print_vec3(t_vec3 a);
int		to_mlxcolor(t_color col);

// ray.c
t_point	ray_at(t_ray ray, double t);
t_color	ray_color(t_ray ray);

#endif
