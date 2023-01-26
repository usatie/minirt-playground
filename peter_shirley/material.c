#include "rtweekend.h"
#include "material.h"
#include <stdlib.h> // calloc

t_lambertian	new_lambertian(t_texture *albedo)
{
	t_lambertian	self = {};

	self.type = LAMBERTIAN;
	self.albedo = albedo;
	return (self);
}

t_metal			new_metal(t_texture *albedo, double fuzz)
{
	t_metal	self = {};

	self.type = METAL;
	self.albedo = albedo;
	self.fuzz = fuzz;
	return (self);
}

t_dielectric	new_dielectric(double ref_idx)
{
	t_metal	self = {};

	self.type = DIELECTRIC;
	self.ref_idx = ref_idx;
	return (self);
}

t_mixed_material	new_mixed_material(t_texture *albedo, double fuzz, double lambertian_ratio)
{
	t_mixed_material	self = {};

	self.type = MIXED_MATERIAL;
	self.albedo = albedo;
	self.fuzz = fuzz;
	self.lambertian_ratio = lambertian_ratio;
	return (self);
}

t_lambertian	*alloc_lambertian(t_texture *albedo)
{
	t_lambertian	*self;

	self = calloc(1, sizeof(*self));
	*self = new_lambertian(albedo);
	return (self);
}

t_metal			*alloc_metal(t_texture *albedo, double fuzz)
{
	t_metal	*self;

	self = calloc(1, sizeof(*self));
	*self = new_metal(albedo, fuzz);
	return (self);
}

t_dielectric	*alloc_dielectric(double ref_idx)
{
	t_dielectric	*self;

	self = calloc(1, sizeof(*self));
	*self = new_dielectric(ref_idx);
	return (self);
}

t_mixed_material	*alloc_mixed_material(t_texture *albedo, double fuzz, double lambertian_ratio)
{
	t_mixed_material	*self;

	self = calloc(1, sizeof(*self));
	*self = new_mixed_material(albedo, fuzz, lambertian_ratio);
	return (self);
}

bool	lambertian_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	t_vec3	scatter_direction;

	(void)r_in;
	scatter_direction = add_vec3(rec->normal, random_unit_vector());
	*scattered = new_ray(rec->p, scatter_direction);
	*attenuation = texture_color_value(self->albedo, rec->u, rec->v, &(rec->p));
	return (true);
}

bool	metal_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	
	t_vec3	v = unit_vec3((r_in->direction));
	t_vec3	reflected = reflect(&v, &(rec->normal));
	*scattered = new_ray(rec->p, add_vec3(reflected, scalar_mul_vec3(self->fuzz, random_in_unit_sphere())));
	*attenuation = texture_color_value(self->albedo, rec->u, rec->v, &(rec->p));
	return (dot_vec3(scattered->direction, rec->normal) > 0);
}

bool	mixed_material_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	double	probability;

	probability = random_double();
	if (probability < self->lambertian_ratio)
		return (lambertian_scatter(self, r_in, rec, attenuation, scattered));
	else
		return (metal_scatter(self, r_in, rec, attenuation, scattered));
}

double	schlick(double cosine, double ref_idx)
{
	double	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1-r0)*pow((1 - cosine), 5));
}

bool	dielectric_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	double etai_over_etat;

	*attenuation = new_color(1.0, 1.0, 1.0);
	if (rec->front_face)
		etai_over_etat = 1.0 / self->ref_idx;
	else
		etai_over_etat = self->ref_idx;
	
	t_vec3	unit_direction = unit_vec3((r_in->direction));
	double	cos_theta = fmin(dot_vec3(scalar_mul_vec3(-1, unit_direction), rec->normal), 1.0);
	double	sin_theta = sqrt(1 - cos_theta * cos_theta);
	if (etai_over_etat * sin_theta > 1.0)
	{
		t_vec3	reflected = reflect(&unit_direction, &rec->normal);
		*scattered = new_ray(rec->p, reflected);
		return (true);
	}
	double	reflect_prob = schlick(cos_theta, etai_over_etat);
	if (random_double() < reflect_prob)
	{
		t_vec3	reflected = reflect(&unit_direction, &rec->normal);
		*scattered = new_ray(rec->p, reflected);
		return (true);
	}
	t_vec3	refracted = refract(&unit_direction, &(rec->normal), etai_over_etat);
	*scattered = new_ray(rec->p, refracted);
	return (true);
}


bool	scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	if (self->type == LAMBERTIAN)
		return (lambertian_scatter(self, r_in, rec, attenuation, scattered));
	else if (self->type == METAL)
		return (metal_scatter(self, r_in, rec, attenuation, scattered));
	else if (self->type == DIELECTRIC)
		return (dielectric_scatter(self, r_in, rec, attenuation, scattered));
	else if (self->type == MIXED_MATERIAL)
		return (mixed_material_scatter(self, r_in, rec, attenuation, scattered));
	return (0);
}

