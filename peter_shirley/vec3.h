#ifndef VEC3_H
# define VEC3_H

# include <math.h>

typedef struct s_vec3	t_vec3;
typedef struct s_vec3	t_point;
typedef struct s_vec3	t_color;

struct s_vec3 {
	double	x;
	double	y;
	double	z;
};

//vec3.c
t_vec3	new_vec3(double x, double y, double z);
t_point	new_point(double x, double y, double z);
t_color	new_color(double x, double y, double z);
t_vec3	add_vec3(t_vec3 a, t_vec3 b);
t_vec3	sub_vec3(t_vec3 a, t_vec3 b);
t_vec3	mul_vec3(t_vec3 a, t_vec3 b);
t_vec3	scalar_mul_vec3(double t, t_vec3 a);
t_vec3	scalar_div_vec3(t_vec3 a, double t);
double	dot_vec3(t_vec3 a, t_vec3 b);
t_vec3	cross_vec3(t_vec3 a, t_vec3 b);
double	length_squared_vec3(t_vec3 a);
double	length_vec3(t_vec3 a);
t_vec3	unit_vec3(t_vec3 a);
void	print_vec3(t_vec3 a);
int		to_mlxcolor(t_color pixel_color, int samples_per_pixel);

#endif
