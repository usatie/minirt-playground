#include <stdlib.h>
#include "playground.h"

t_fcolor	*fcolor_new(float red, float green, float blue)
{
	t_fcolor	*color;
	
	color = calloc(1, sizeof(*color));
	color->red = red;
	color->green = green;
	color->blue = blue;
	return (color);
}

void	fcolor_set(t_fcolor *c, float red, float green, float blue)
{
	c->red = red;
	c->green = green;
	c->blue = blue;
}

t_fcolor	*fcolor_copy(t_fcolor *c)
{
	return (fcolor_new(c->red, c->green, c->blue));
}

t_fcolor	*fcolor_add(t_fcolor *c1, t_fcolor *c2)
{
	t_fcolor	*color;

	color = fcolor_copy(c1);
	color->red += c2->red;
	color->green += c2->green;
	color->blue += c2->blue;
	return (color);
}

t_fcolor	*fcolor_mul(t_fcolor *c1, t_fcolor *c2)
{
	t_fcolor	*color;

	color = fcolor_copy(c1);
	color->red *= c2->red;
	color->green *= c2->green;
	color->blue *= c2->blue;
	return (color);
}
