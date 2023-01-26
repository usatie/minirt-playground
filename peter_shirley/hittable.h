#ifndef HITTABLE_H
# define HITTABLE_H

# include <stdbool.h>
# include "ray.h"
# include "aabb.h"

typedef struct s_hit_record	t_hit_record;
typedef struct s_hittable	t_hittable;
typedef struct s_material	t_material;

enum e_hittable_type {
	SPHERE,
	XY_RECT,
};
typedef enum e_hittable_type	t_hittable_type;

struct s_hit_record {
	t_point		p;
	t_vec3		normal;
	t_material	*mat_ptr;
	double		t;
	double		u;
	double		v;
	bool		front_face;
};

struct s_hittable {
	t_hittable_type	type;
	t_material		*mat_ptr;
	// SPHERE
	t_point			center;
	double			radius;
	// XY_RECT
	double			x0, x1, y0, y1, k;
	//HITTABLE_LIST
	t_hittable		*next;
};

bool	bounding_box(const t_hittable *self, t_aabb *output_box);
bool	hit_single(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec);
void	set_face_normal(t_hit_record *self, const t_ray *r, const t_vec3 *outward_normal);

#endif
