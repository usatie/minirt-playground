#ifndef COLOR_H
# define COLOR_H

# include <stdint.h>
typedef union u_rgb		t_rgb;

t_rgb	rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha);
t_rgb	rgb_mul(t_rgb col, double alpha);
t_rgb	rgb_add(t_rgb col1, t_rgb col2);

t_rgb	red(void);
t_rgb	green(void);
t_rgb	blue(void);
t_rgb	black(void);
t_rgb	white(void);

/*
** r — Red
** g — Green
** b — Blue
** alpha — Alpha
*/
struct s_rgb {
	uint8_t	b:8;
	uint8_t	g:8;
	uint8_t	r:8;
	uint8_t	alpha:8;
};

union u_rgb {
	struct s_rgb	rgb;
	int32_t			mlx_color;
};
#endif
