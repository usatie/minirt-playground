#include <stdlib.h>
#include "playground.h"

t_shape	*shape_new(t_shape_kind kind)
{
	t_shape	*sp;

	sp = calloc(1, sizeof(*sp));
	sp->kind = kind;
	return (sp);
}

