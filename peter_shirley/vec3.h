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
t_vec3	random_vec3(void);
t_vec3	random_range_vec3(double min, double max);
t_vec3	random_in_unit_sphere(void);
t_vec3	random_unit_vector();
t_vec3	random_in_hemisphere(const t_vec3 *normal);
t_vec3	reflect(const t_vec3 *v, const t_vec3 *n);

#endif
