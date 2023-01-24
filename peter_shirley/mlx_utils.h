#ifndef MLX_UTILS_H
# define MLX_UTILS_H

typedef struct s_img	t_img;
typedef struct s_screen	t_screen;

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

// mlx_utils.c
t_screen	*init_screen(void *mlx_ptr);
void		*init_img(void *mlx_ptr, int width, int height);
void		put_pixel(const t_img *img, int x, int y, int mlx_color);

#endif
