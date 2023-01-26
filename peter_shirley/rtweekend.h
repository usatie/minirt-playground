#ifndef RTWEEKEND_H
# define RTWEEKEND_H

# define ASPECT_RATIO (16.0 / 9.0)
# define WIN_WIDTH 384
# define WIN_HEIGHT (int)(WIN_WIDTH / ASPECT_RATIO)

# include <stdbool.h>
# include <stdio.h>
# include <math.h>
# include "mlx_utils.h"
# include "color.h"
# include "vec3.h"
# include "ray.h"

typedef struct s_env	t_env;

struct s_env {
	void		*mlx_ptr;
	t_screen	*screen;
};

double	degrees_to_radians(double degrees);
double	random_double(void);
double	random_double_range(double min, double max);
int		random_int_range(int min, int max);
double	clamp(double x, double min, double max);


#endif
