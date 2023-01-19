#ifndef PLAYGROUND_H
# define PLAYGROUND_H
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
pvector	*pvector_new(float x, float y, float z);
void	pvector_set(pvector *v, float x, float y, float z);
pvector	*pvector_copy(pvector *v);
pvector	*pvector_add(pvector *v1, pvector *v2);
pvector	*pvector_sub(pvector *v1, pvector *v2);
pvector	*pvector_mul(pvector *v, float k);
float	pvector_dot(pvector *v1, pvector *v2);
float	pvector_magsq(pvector *v);
float	pvector_mag(pvector *v);
void	pvector_normalize(pvector *v);
void	pvector_print(pvector *v);

#endif
