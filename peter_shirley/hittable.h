#ifndef HITTABLE_H
# define HITTABLE_H

# include <stdbool.h>
# include "ray.h"
# include "aabb.h"

typedef struct s_hit_record	t_hit_record;
typedef struct s_hittable	t_hittable;
typedef struct s_material	t_material;

typedef t_hittable	t_sphere;
typedef t_hittable	t_xy_rect;
typedef t_hittable	t_yz_rect;
typedef t_hittable	t_xz_rect;
typedef t_hittable t_hittable_list;
typedef t_hittable	t_bvh_node;
typedef bool	t_comparator(t_hittable *a, t_hittable *b);

enum e_hittable_type {
	SPHERE,
	XY_RECT,
	YZ_RECT,
	XZ_RECT,
	HITTABLE_LIST,
	BVH_NODE,
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
	double			x0, x1, y0, y1, z0, z1, k;
	//HITTABLE_LIST
	t_hittable		*next;
	// BVH_NODE
	t_hittable		*left;
	t_hittable		*right;
	t_aabb			box;
};

void	set_face_normal(t_hit_record *self, const t_ray *r, const t_vec3 *outward_normal);
bool	hit(const t_hittable_list *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec);
bool	bounding_box(const t_hittable *self, t_aabb *output_box);

// new
t_sphere	sphere_new(t_point cen, double r, t_material *m);
t_xy_rect	xyrect_new(double x0, double x1, double y0, double y1, double k, t_material *m);
t_bvh_node	new_bvh_node(t_hittable_list *s, t_hittable_list *e);

// alloc
t_sphere	*sphere_alloc(t_point cen, double r, t_material *m);
t_xy_rect	*xyrect_alloc(double x0, double x1, double y0, double y1, double k, t_material *m);
t_bvh_node	*alloc_bvh_node(t_hittable_list *s, t_hittable_list *e);
t_yz_rect	*yzrect_alloc(double y0, double y1, double z0, double z1, double k, t_material *m);
t_xz_rect	*xzrect_alloc(double x0, double x1, double z0, double z1, double k, t_material *m);
// hittable_list
void	hittable_list_add(t_hittable_list *self, t_hittable *object);
size_t	span_of_hittable_list(t_hittable_list *start, t_hittable_list *end);

#endif
