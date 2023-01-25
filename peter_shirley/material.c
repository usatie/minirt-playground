#include "material.h"

bool	lambertian_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	t_vec3	scatter_direction;

	(void)r_in;
	scatter_direction = add_vec3(rec->normal, random_unit_vector());
	*scattered = new_ray(rec->p, scatter_direction);
	*attenuation = self->albedo;
	return (true);
}

bool	metal_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	
	t_vec3	v = unit_vec3((r_in->direction));
	t_vec3	reflected = reflect(&v, &(rec->normal));
	*scattered = new_ray(rec->p, add_vec3(reflected, scalar_mul_vec3(self->fuzz, random_in_unit_sphere())));
	*attenuation = self->albedo;
	return (dot_vec3(scattered->direction, rec->normal) > 0);
}

bool	dielectric_scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	
	double etai_over_etat;
	if (rec->front_face)
		etai_over_etat = 1.0 / self->ref_idx;
	else
		etai_over_etat = self->ref_idx;
	
	t_vec3	v = unit_vec3((r_in->direction));
	t_vec3	refracted = refract(&v, &(rec->normal), etai_over_etat);
	*scattered = new_ray(rec->p, refracted);
	*attenuation = new_color(1.0, 1.0, 1.0);
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
	return (0);
}
