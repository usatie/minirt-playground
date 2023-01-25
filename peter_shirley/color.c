#include "color.h"

t_rgb	rgb_mul(t_rgb col, double alpha)
{
	col.rgb.r *= alpha;
	col.rgb.g *= alpha;
	col.rgb.b *= alpha;
	return (col);
}

t_rgb	rgb_add(t_rgb col1, t_rgb col2)
{
	t_rgb	color;

	if (255 - col1.rgb.r > col2.rgb.r)
		color.rgb.r = 255;
	else
		color.rgb.r = col1.rgb.r + col2.rgb.r;
	if (255 - col1.rgb.g > col2.rgb.g)
		color.rgb.g = 255;
	else
		color.rgb.g = col1.rgb.g + col2.rgb.g;
	if (255 - col1.rgb.b > col2.rgb.b)
		color.rgb.b = 255;
	else
		color.rgb.b = col1.rgb.b + col2.rgb.b;
	return (color);
}

t_rgb	rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	t_rgb	color;

	color.rgb.r = r;
	color.rgb.b = b;
	color.rgb.g = g;
	color.rgb.alpha = alpha;
	return (color);
}

t_rgb	red(void)
{
	return (rgb(255, 0, 0, 0));
}

t_rgb	green(void)
{
	return (rgb(0, 255, 0, 0));
}

t_rgb	blue(void)
{
	return (rgb(0, 0, 255, 0));
}

t_rgb	black(void)
{
	return (rgb(0, 0, 0, 0));
}

t_rgb	white(void)
{
	return (rgb(255, 255, 255, 255));
}
