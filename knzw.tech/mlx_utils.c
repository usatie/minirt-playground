#include <stdlib.h>
#include "playground.h"
#include "mlx.h"

t_screen	*init_screen(void *mlx_ptr)
{
	t_screen	*screen;

	screen = calloc(1, sizeof(*screen));
	screen->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "screen");
	screen->img = init_img(mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	return (screen);
}

void	*init_img(void *mlx_ptr, int width, int height)
{
	t_img	*img;

	img = malloc(sizeof(t_img));
	if (img == NULL)
		return (NULL);
	img->ptr = mlx_new_image(mlx_ptr, width, height);
	if (img->ptr == NULL)
	{
		free(img);
		return (NULL);
	}
	img->mlx_ptr = mlx_ptr;
	img->data = mlx_get_data_addr(
			img->ptr,
			&img->bits_per_pixel,
			&img->bytes_per_line,
			&img->endian);
	img->bytes_per_pixel = img->bits_per_pixel / 8;
	return (img);
}

static char	get_n_th_byte(int x, int n)
{
	const int	size = 8;
	const int	index = n * size;
	int			mask;
	char		ret;

	mask = ((n << size) - 1) << index;
	ret = (char)((x & mask) >> index);
	return (ret);
}

// Update img data
// If endian is little endian, write from head,
// Else write from tail.
void	put_pixel(const t_img *img, int x, int y, int mlx_color)
{
	unsigned int	x_color;
	int	head;
	int	i;
	int	index;

	x_color = mlx_color;
	x_color = mlx_get_color_value(img->mlx_ptr, mlx_color);
	head = (x * img->bytes_per_pixel) + (y * img->bytes_per_line);
	i = 0;
	while (i < img->bytes_per_pixel)
	{
		if (img->endian == 0)
		{
			index = head + i;
		}
		else
		{
			index = head + img->bytes_per_pixel - 1 - i;
		}
		img->data[index] = get_n_th_byte(x_color, i);
		i++;
	}
}
