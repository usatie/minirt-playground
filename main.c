#include <stdio.h>
#include "playground.h"

int	main(void)
{
	printf("constrain(-1, 0, 10) = %f\n", constrain(-1, 0, 10));
	printf("map(5, 0, 10, 0, 1) = %f\n", map(5, 0, 10, 0, 1));

	pvector	*v1;
	pvector	*v2;

	v1 = pvector_new(1.0, 2.0, 3.0);
	v2 = pvector_copy(v1);
	printf("v1: ");
	pvector_print(v1);
	printf("\n");

	pvector_normalize(v2);

	printf("v2: ");
	pvector_print(v2);
	printf("\n");
	return (0);
}
