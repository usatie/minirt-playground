#ifndef PERLIN_H
# define PERLIN_H
# include "rtweekend.h"

typedef	struct s_perlin t_perlin;

struct s_perlin {
	double	ranfloat[256];
	int		x[256];
	int		y[256];
	int		z[256];
};

void	perlin_generate_perm(int *p);
double	perlin_noise(const t_perlin *self, const t_point *p);
t_perlin	new_perlin(void);
#endif
