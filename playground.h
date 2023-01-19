#ifndef PLAYGROUND_H
# define PLAYGROUND_H
typedef struct point	point;
typedef struct pvector	pvector;
typedef struct s_img	t_img;
typedef struct s_screen	t_screen;
typedef struct s_env	t_env;

struct point {
	int	x;
	int	y;
};

struct pvector {
	float	x;
	float	y;
	float	z;
};

struct s_img {
	void	*mlx_ptr;
	void	*ptr;
	char	*data;
	int		bits_per_pixel;
	int		bytes_per_pixel;
	int		bytes_per_line;
	int		endian;
};

struct s_screen {
	void			*win_ptr;
	t_img			*img;
};

struct s_env {
	void		*mlx_ptr;
	t_screen	*screen;
};

float constrain(float v, float vmin, float vmax); // 値の制限
float map(float v, float vmin, float vmax, float tmin, float tmax); // 値のマッピング
pvector	*pvector_new(float x, float y, float z);
void	pvector_set(pvector *v, float x, float y, float z);
pvector	*pvector_copy(pvector *v);
pvector	*pvector_add(pvector *v1, pvector *v2);
pvector	*pvector_sub(pvector *v1, pvector *v2);
pvector	*pvector_mul(pvector *v, float k);
float	pvector_dot(pvector *v1, pvector *v2);
float	pvector_magsq(pvector *v);
float	pvector_mag(pvector *v);
void	pvector_normalize(pvector *v);
void	pvector_print(pvector *v);

#endif
