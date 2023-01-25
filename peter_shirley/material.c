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

bool	scatter(const t_material *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered)
{
	// if lambertian
	return (lambertian_scatter(self, r_in, rec, attenuation, scattered));
	// if metal
	// return (metal_scatter(self, r_in, rec, attenuation, scattered));
}
