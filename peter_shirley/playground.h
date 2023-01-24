#ifndef PLAYGROUND_H
# define PLAYGROUND_H

# define WIN_WIDTH 800
# define WIN_HEIGHT 512

# include <stdbool.h>
# include "mlx_utils.h"
# include "color.h"

typedef struct s_env	t_env;

struct s_env {
	void		*mlx_ptr;
	t_screen	*screen;
};

#endif
