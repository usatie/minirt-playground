#include <stdlib.h>
#include "playground.h"

t_material	*material_new(void)
{
	t_material	*material;

	material = calloc(1, sizeof(*material));
	return (material);
}
