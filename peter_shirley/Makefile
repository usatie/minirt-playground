#############
# Variables #
#############

CC			=	cc
NAME		=	a.out

LIBS		=	-lmlx -lXext -lX11 -lm
MLX_DIR		=	minilibx-linux
MLX			=	$(MLX_DIR)/libmlx.a
LIBPATH		=	-L$(MLX_DIR) -L/usr/X11R6/lib
INCLUDE_DIR	=	include
INCLUDES	=	-I $(INCLUDE_DIR) -I $(MLX_DIR) -I /usr/X11R6/include
#CFLAGS		=	-Wall -Werror -Wextra
DFLAGS		=	-g -fsanitize=address
HEADERS		=	rtweekend.h color.h mlx_utils.h ray.h vec3.h hittable.h camera.h material.h texture.h aabb.h perlin.h
SRCS		=	main.c mlx_utils.c color.c vec3.c ray.c hittable.c camera.c material.c texture.c aabb.c perlin.c
FRAMEWORK = -framework OpenGL -framework AppKit

$(ALL): $(NAME)

$(NAME): $(SRCS) $(MLX) $(HEADERS)
	$(CC) $(SRCS) $(INCLUDES) $(CFLAGS) $(LIBS) $(LIBPATH) $(FRAMEWORK) -o $(NAME)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

debug:	CFLAGS+=$(DFLAGS)
debug:	$(NAME)