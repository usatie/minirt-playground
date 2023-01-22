#include <stdlib.h>
#include "playground.h"

t_light_source *light_source_new(t_light_kind kind)
{
	t_light_source *src;

	src = calloc(1, sizeof(*src));
	src->kind = kind;
	return (src);
}

t_lighting *lighting_at(pvector *pos, t_light_source *light_source)
{
	t_lighting	*lighting;

	lighting = calloc(1, sizeof(*lighting));
	lighting->direction = pvector_sub(light_source->position, pos);
	lighting->distance = pvector_mag(lighting->direction);
	pvector_normalize(lighting->direction);
	lighting->intencity = light_source->intencity;
	return (lighting);
}
