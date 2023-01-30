#ifndef TEXTURE_H
#define TEXTURE_H
#include "rtweekend.h"
#include "perlin.h"

typedef	struct s_texture t_texture;
typedef	t_texture t_solid_color;
typedef	t_texture t_checker_texture;
typedef	t_texture t_noise_texture;

enum	e_texture_type {
	SOLID_COLOR,
	CHECKER_TEXTURE,
	NOISE_TEXTURE,
};
typedef enum e_texture_type	t_texture_type;

struct s_texture {
	t_texture_type	type;
	// SOLID_COLOR
	t_color			color_val;
	// CHECKER_TEXTURE
	t_texture		*even;
	t_texture		*odd;
	//PERLIN_NOISE_TEXTURE
	t_perlin		noise;
};

t_color		texture_color_value(const t_texture *self, double u, double v, const t_vec3 *p);

// solid_color
t_solid_color	new_solid_color(double r, double g, double b);
t_solid_color	*alloc_solid_color(double r, double g, double b);

// checker_texture
t_checker_texture	new_checker_texture(t_texture *t0, t_texture *t1);
t_checker_texture	*alloc_checker_texture(t_texture *t0, t_texture *t1);

//noise_texture
t_noise_texture	*alloc_noise_texture(void);

#endif
