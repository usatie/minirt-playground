#ifndef MATERIAL_H
# define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

typedef struct s_material	t_material;
typedef t_material	t_lambertian;
typedef t_material	t_metal;
typedef t_material	t_dielectric;
//typedef enum e_mat_type	t_mat_type;

typedef enum e_mat_type {
	LAMBERTIAN,
	METAL,
	DIELECTRIC
} t_mat_type;

struct s_material {
	t_mat_type type;
	// lambertian
	t_color	albedo;
	//metal
	double	fuzz;
	//dielectric
	double	ref_idx;
};

bool	scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered);

t_lambertian	new_lambertian(t_color albedo);
t_metal			new_metal(t_color albedo, double fuzz);
t_dielectric	new_dielectric(double ref_idx);

t_lambertian	*alloc_lambertian(t_color albedo);
t_metal			*alloc_metal(t_color albedo, double fuzz);
t_dielectric	*alloc_dielectric(double ref_idx);

#endif
