#ifndef MATERIAL_H
# define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

typedef struct s_material	t_material;
typedef t_material	t_lambertian;
typedef t_material	t_metal;
typedef t_material	t_dielectric;
typedef t_material	t_mixed_material;
typedef t_material	t_diffuse_light;
typedef t_material	t_isotropic;
//typedef enum e_mat_type	t_mat_type;

typedef enum e_mat_type {
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
	MIXED_MATERIAL,
	DIFFUSE_LIGHT,
	ISOTROPIC,
} t_mat_type;

struct s_material {
	t_mat_type	type;
	// lambertian
	t_texture	*albedo;
	//metal
	double		fuzz;
	//dielectric
	double		ref_idx;
	// mixed
	double		lambertian_ratio;
	//diffused light
	t_texture	*emit;
};

bool	scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered);

t_lambertian		new_lambertian(t_texture *albedo);
t_metal				new_metal(t_texture *albedo, double fuzz);
t_dielectric		new_dielectric(double ref_idx);
t_mixed_material	new_mixed_material(t_texture *albedo, double fuzz, double lambertian_ratio);
t_diffuse_light		new_diffuse_light(t_texture *emit);
t_isotropic			new_isotropic(t_texture *albedo);

t_lambertian		*alloc_lambertian(t_texture *albedo);
t_metal				*alloc_metal(t_texture *albedo, double fuzz);
t_dielectric		*alloc_dielectric(double ref_idx);
t_mixed_material	*alloc_mixed_material(t_texture *albedo, double fuzz, double lambertian_ratio);
t_diffuse_light		*alloc_diffuse_light(t_texture *emit);
t_isotropic			*alloc_isotropic(t_texture *albedo);

t_color				material_emitted(t_material *self, double u, double v, const t_point *p);

#endif
