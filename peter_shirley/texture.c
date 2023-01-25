#include "texture.h"
#include <stdlib.h>

t_solid_color	new_solid_color(double r, double g, double b)
{
	t_solid_color	ret;

	ret.color_val = new_color(r, g, b);
	return (ret);
}

t_solid_color	*alloc_solid_color(double r, double g, double b)
{
	t_solid_color *self = calloc(1, sizeof(*self));

	(*self) = new_solid_color(r, g, b);
	return (self);
}


t_color	texture_color_value(t_texture *self, double u, double v, const t_vec3 *p)
{
	//solid_color
	return (self->color_val);
}