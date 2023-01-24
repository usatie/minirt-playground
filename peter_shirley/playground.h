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
typedef struct t_vec3	t_color;

struct s_env {
	void		*mlx_ptr;
	t_screen	*screen;
};

struct s_vec3 {
	double	x;
	double	y;
	double	z;
};

//vec3.c
t_vec3	new_vec3(double x, double y, double z);
t_vec3	add_vec3(t_vec3 a, t_vec3 b);
t_vec3	sub_vec3(t_vec3 a, t_vec3 b);
double	dot_vec3(t_vec3 a, t_vec3 b);
t_vec3	t_vec3_cross(t_vec3 *a, t_vec3 *b);
double	mag_of_vec3(t_vec3 a);
t_vec3	unit_vec3(t_vec3 a);
void	print_vec3(t_vec3 a);

#endif
