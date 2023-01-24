#include <stdlib.h>
#include "playground.h"

t_ray	*ray_new(pvector *start, pvector *direction)
{
	t_ray	*ray;

	ray = calloc(1, sizeof(*ray));
	ray->start = start;
	ray->direction = direction;
	return (ray);
}

