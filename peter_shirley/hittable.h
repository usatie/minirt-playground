#ifndef HITTABLE_H
# define HITTABLE_H

# include <stdbool.h>
# include "ray.h"

typedef struct s_hit_record	t_hit_record;
typedef struct s_hittable	t_hittable;
typedef struct s_material	t_material;

struct s_hit_record {
	t_point		p;
	t_vec3		normal;
	t_material	*mat_ptr;
	double		t;
	bool		front_face;
};

struct s_hittable {
	t_material	*mat_ptr;
	// SPHERE
	t_point		center;
	double		radius;
	//HITTABLE_LIST
	t_hittable	*next;
};

bool	hit_single(const t_hittable *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec);
void	set_face_normal(t_hit_record *self, const t_ray *r, const t_vec3 *outward_normal);

#endif
