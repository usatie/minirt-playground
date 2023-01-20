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
CFLAGS		=	-Wall -Werror -Wextra
SRCS		=	main.c vector.c mlx_utils.c color.c fcolor.c
FRAMEWORK = -framework OpenGL -framework AppKit

$(NAME): $(SRCS) $(MLX)
	$(CC) $(SRCS) $(INCLUDES) $(CFLAGS) $(LIBS) $(LIBPATH) $(FRAMEWORK) -o $(NAME)

$(MLX):
	$(MAKE) -C $(MLX_DIR)
