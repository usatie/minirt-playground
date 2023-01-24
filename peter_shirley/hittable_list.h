#ifndef HITTABLE_LIST_H
# define HITTABLE_LIST_H
# include <stdlib.h>
# include "hittable.h"

typedef t_hittable t_hittable_list;

void	add(t_hittable_list *self, t_hittable *object);
bool	hit(const t_hittable_list *self, const t_ray *r, double t_min, double t_max, t_hit_record *rec);

#endif
