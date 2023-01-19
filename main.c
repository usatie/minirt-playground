#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct point	point;
typedef struct pvector	pvector;

struct point {
	int	x;
	int	y;
};

struct pvector {
	float	x;
	float	y;
	float	z;
};

float constrain(float v, float vmin, float vmax); // 値の制限
float map(float v, float vmin, float vmax, float tmin, float tmax); // 値のマッピング

float constrain(float v, float vmin, float vmax)
{
	if (v > vmax)
		return (vmax);
	else if (v < vmin)
		return (vmin);
	else
		return (v);
}

// map(𝑣,𝑣𝑚𝑖𝑛,𝑣𝑚𝑎𝑥,𝑡𝑚𝑖𝑛,𝑡𝑚𝑎𝑥) = 𝑡𝑚𝑖𝑛 + (𝑡𝑚𝑎𝑥–𝑡𝑚𝑖𝑛) × constrain(𝑣,𝑣𝑚𝑖𝑛,𝑣𝑚𝑎𝑥) / 𝑣𝑚𝑎𝑥−𝑣𝑚𝑖𝑛
float map(float v, float vmin, float vmax, float tmin, float tmax)
{
	return (tmin + (tmax - tmin) * constrain(v, vmin, vmax) / (vmax - vmin));
}

pvector	*pvector_new(float x, float y, float z)
{
	pvector	*newv;
	
	newv = calloc(1, sizeof(*newv));
	if (newv == NULL)
		exit(1);
	newv->x = x;
	newv->y = y;
	newv->z = z;
	return (newv);
}

void	pvector_set(pvector *v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

pvector	*pvector_copy(pvector *v)
{
	pvector	*newv;
	
	newv = pvector_new(v->x, v->y, v->z);
	return (newv);
}

pvector	*pvector_add(pvector *v1, pvector *v2)
{
	pvector	*newv;

	newv = pvector_new(0, 0, 0);
	pvector_set(newv, v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
	return (newv);
}

pvector	*pvector_sub(pvector *v1, pvector *v2)
{
	pvector	*newv;

	newv = pvector_new(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
	return (newv);
}

pvector	*pvector_mul(pvector *v, float k)
{
	pvector	*newv;

	newv = pvector_new(v->x * k, v->y * k, v->z * k);
	return (newv);
}

float	pvector_dot(pvector *v1, pvector *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

float	pvector_magsq(pvector *v)
{
	return (v->x * v->x + v->y * v->y + v->z * v->z);
}

float	pvector_mag(pvector *v)
{
	return (sqrt(pvector_magsq(v)));
}

void	pvector_normalize(pvector *v)
{
	float	norm;

	norm = pvector_mag(v);
	if (norm != 0)
		pvector_set(v, v->x / norm, v->y / norm, v->z / norm);
}

void	pvector_print(pvector *v)
{
	printf("[ %f, %f, %f ]", v->x, v->y, v->z);
}

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
