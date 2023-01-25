#ifndef TEXTURE_H
#define TEXTURE_H
#include "rtweekend.h"


typedef	struct s_texture t_texture;
typedef	t_texture t_solid_color;

struct s_texture {
	t_color	color_val;
};

t_solid_color	new_solid_color(double r, double g, double b);
t_solid_color	*alloc_solid_color(double r, double g, double b);
t_color		texture_color_value(t_texture *self, double u, double v, const t_vec3 *p);

#endif