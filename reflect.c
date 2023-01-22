#include <math.h>
#include "playground.h"

t_fcolor	*ambient_light(t_ray *ray, t_shape *shape)
{
	t_fcolor	*ambient_intensity;
	
	(void)ray;
	ambient_intensity = fcolor_new(0.1, 0.1, 0.1);
	return (fcolor_mul(ambient_intensity, shape->material->amibient_factor));
}

t_fcolor	*diffuse_light(t_shape *shape, t_intersection_point *intersection, t_lighting *lighting)
{
	t_fcolor	*color;
	pvector	*n;
	pvector	*l;
	float	nldot;

	n = intersection->normal;
	l = lighting->direction;
	nldot = constrain(pvector_dot(l, n), 0 , 1);
	color = fcolor_mul(lighting->intencity, shape->material->diffuse_factor);
	color = fcolor_mul(color, fcolor_new(nldot, nldot, nldot));
	return (color);
}

t_fcolor	*specular_light(t_ray *ray, t_shape *shape, t_intersection_point *intersection, t_lighting *lighting)
{
	pvector	*reflection;
	pvector *b;
	pvector	*view;
	float	nldot;
	float	vrdot;
	float	vrdot_pow_alpha;
	t_fcolor	*ki;

	nldot = pvector_dot(intersection->normal, lighting->direction);
	b = pvector_mul(intersection->normal, nldot * 2);
	reflection = pvector_sub(b, lighting->direction);
	view = pvector_mul(ray->direction, -1);
	pvector_normalize(view);
	vrdot = pvector_dot(view, reflection);
	if (nldot <= 0 || vrdot <= 0)
		return (fcolor_new(0, 0, 0));
	vrdot_pow_alpha = pow(vrdot, shape->material->shineness);
	ki = fcolor_mul(shape->material->specular_factor, lighting->intencity);
	return (fcolor_mul(ki, fcolor_new(vrdot_pow_alpha, vrdot_pow_alpha, vrdot_pow_alpha)));
}
