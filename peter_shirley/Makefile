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
HEADERS		=	playground.h color.h mlx_utils.h ray.h vec3.h sphere.h hittable.h
SRCS		=	main.c mlx_utils.c color.c vec3.c ray.c hittable.c sphere.c hittable_list.c
FRAMEWORK = -framework OpenGL -framework AppKit

$(NAME): $(SRCS) $(MLX) $(HEADERS)
	$(CC) $(SRCS) $(INCLUDES) $(CFLAGS) $(LIBS) $(LIBPATH) $(FRAMEWORK) -o $(NAME)

$(MLX):
	$(MAKE) -C $(MLX_DIR)
