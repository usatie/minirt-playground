#include "texture.h"
#include <stdlib.h>

t_solid_color	new_solid_color(double r, double g, double b)
{
	t_solid_color	self;

	self.type = SOLID_COLOR;
	self.color_val = new_color(r, g, b);
	return (self);
}

t_solid_color	*alloc_solid_color(double r, double g, double b)
{
	t_solid_color *self = calloc(1, sizeof(*self));

	(*self) = new_solid_color(r, g, b);
	return (self);
}

t_checker_texture	new_checker_texture(t_texture *t0, t_texture *t1)
{
	t_solid_color	self;

	self.type = CHECKER_TEXTURE;
	self.even = t0;
	self.odd = t1;
	return (self);
}

t_checker_texture	*alloc_checker_texture(t_texture *t0, t_texture *t1)
{
	t_solid_color *self = calloc(1, sizeof(*self));

	(*self) = new_checker_texture(t0, t1);
	return (self);
}

t_color	solid_color_value(t_solid_color *self, double u, double v, const t_vec3 *p)
{
	(void)u;
	(void)v;
	(void)p;
	return (self->color_val);
}

t_color	checker_texture_value(t_solid_color *self, double u, double v, const t_vec3 *p)
{
	const double	sines = sin(10 * p->x) * sin(10 * p->y) * sin(10 * p->z);

	if (sines < 0)
		return (texture_color_value(self->odd, u, v, p));
	else
		return (texture_color_value(self->even, u, v, p));
}

t_color	texture_color_value(t_texture *self, double u, double v, const t_vec3 *p)
{
	if (self->type == SOLID_COLOR)
		return (solid_color_value(self, u, v, p));
	else if (self->type == CHECKER_TEXTURE)
		return (checker_texture_value(self, u, v, p));
	return (self->color_val);
}
