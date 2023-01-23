#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "playground.h"
#include "color.h"
#include "mlx.h"

t_material	*get_default_material(t_fcolor	*color)
{
	t_material		*material;

	material = material_new();
	material->amibient_factor = fcolor_new(0.01, 0.01, 0.01);
	material->diffuse_factor = color;
	material->specular_factor = fcolor_new(0.30, 0.30, 0.30);
	material->shineness = 8.0;
	material->use_perfect_reflectance = false;
	material->catadioptric_factor = fcolor_new(0, 0, 0);
	return (material);
}

t_shape	*get_shapes(void)
{
	t_shape		head;
	t_shape		*shape;
	
	head.next = NULL;
	shape = &head;
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(3, 0, 25);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0.69, 0, 0));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(2, 0, 20);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0, 0.69, 0));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(1, 0, 15);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0, 0, 0.69));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(0, 0, 10);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0, 0.69, 0.69));
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(-1, 0, 5);
		shape->radius = 1.0;
		shape->material = get_default_material(fcolor_new(0.69, 0, 0.69));
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->center = pvector_new(0, -1, 0);
		shape->normal = pvector_new(0, 1, 0);
		shape->material = get_default_material(fcolor_new(0.69, 0.69, 0.69));
	}
	return (head.next);
}

t_light_source	*get_light_sources(void)
{
	t_light_source	head;
	t_light_source	*ls;

	head.next = NULL;
	ls = &head;
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(-5, 5, -5);
		ls->intencity = fcolor_new(0.5, 0.5, 0.5);
	}
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(5, 0, -5);
		ls->intencity = fcolor_new(0.5, 0.5, 0.5);
	}
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(5, 20, -5);
		ls->intencity = fcolor_new(0.5, 0.5, 0.5);
	}
	return (head.next);
}

t_scene	*get_scene(void)
{
	t_scene	*scene;

	scene = calloc(1, sizeof(*scene));
	scene->shapes = get_shapes();
	scene->light_sources = get_light_sources();
	return (scene);
}

t_shape	*get_shapes2(void)
{
	t_shape		head;
	t_shape		*shape;
	
	head.next = NULL;
	shape = &head;
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(-0.4, -0.65, 3.0);
		shape->radius = 0.35;
		shape->material = get_default_material(fcolor_new(0, 0, 0));
		shape->material->amibient_factor = fcolor_new(0, 0, 0);
		shape->material->diffuse_factor = fcolor_new(0, 0, 0);
		shape->material->specular_factor = fcolor_new(0, 0, 0);
		shape->material->shineness = 0;
		shape->material->use_perfect_reflectance = true;
		shape->material->catadioptric_factor = fcolor_new(1.0, 1.0, 1.0);
		
	}
	{
		shape = shape->next = shape_new(SPHERE);
		shape->center = pvector_new(0.5, -0.65, 2.0);
		shape->radius = 0.35;
		shape->material = get_default_material(fcolor_new(0, 0, 0));
		shape->material->amibient_factor = fcolor_new(0, 0, 0);
		shape->material->diffuse_factor = fcolor_new(0, 0, 0);
		shape->material->specular_factor = fcolor_new(0, 0, 0);
		shape->material->shineness = 0;
		shape->material->use_refraction = true;
		shape->material->refraction = 1.51;
		shape->material->catadioptric_factor = fcolor_new(1.0, 1.0, 1.0);
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->normal = pvector_new(0, 1, 0);
		shape->center = pvector_new(0, -1, 0);
		shape->material = get_default_material(fcolor_new(1, 1, 1));
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->normal = pvector_new(0, -1, 0);
		shape->center = pvector_new(0, 1, 0);
		shape->material = get_default_material(fcolor_new(1, 1, 1));
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->normal = pvector_new(-1, 0, 0);
		shape->center = pvector_new(1, 0, 0);
		shape->material = get_default_material(fcolor_new(0, 1, 0));
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->normal = pvector_new(1, 0, 0);
		shape->center = pvector_new(-1, 0, 0);
		shape->material = get_default_material(fcolor_new(1, 0, 0));
	}
	{
		shape = shape->next = shape_new(PLANE);
		shape->normal = pvector_new(0, 0, -1);
		shape->center = pvector_new(0, 0, 5);
		shape->material = get_default_material(fcolor_new(1, 1, 1));
	}
	return (head.next);
}

t_light_source	*get_light_sources2(void)
{
	t_light_source	head;
	t_light_source	*ls;

	head.next = NULL;
	ls = &head;
	{
		ls = ls->next = light_source_new(POINT);
		ls->position = pvector_new(0, 0.9, 2.5);
		ls->intencity = fcolor_new(1.0, 1.0, 1.0);
	}
	return (head.next);
}

t_scene	*get_scene2(void)
{
	t_scene	*scene;

	scene = calloc(1, sizeof(*scene));
	scene->air_refraction = 1.0;
	scene->shapes = get_shapes2();
	scene->light_sources = get_light_sources2();
	return (scene);
}
