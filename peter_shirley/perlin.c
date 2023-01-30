#include "perlin.h"

void	perlin_generate_perm(int *p)
{
	for (int i = 0; i < 256; i++)
		p[i] = i;
	
	for (int i = 0; i < 256; i++)
	{
		int target = random_int_range(0, i);
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

double	perlin_noise(const t_perlin *self, const t_point *p)
{
	int	i = (int)(4 * p->x) & 255;
	int	j = (int)(4 * p->y) & 255;
	int	k = (int)(4 * p->z) & 255;
	return (self->ranfloat[self->x[i] ^ self->y[j] ^ self->z[k]]);
}

t_perlin	new_perlin(void)
{
	t_perlin	self;

	for (int i = 0; i < 256; i++)
	{
		self.ranfloat[i] = random_double();
	}
	perlin_generate_perm(self.x);
	perlin_generate_perm(self.y);
	perlin_generate_perm(self.z);
	return (self);
}
