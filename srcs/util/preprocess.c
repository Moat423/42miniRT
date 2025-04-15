#include "../../include/miniRT.h"
#include <stdbool.h>

int	ft_malloc_objects(t_objects *objects);

/// Returns the distance² after which there is no light contribution from
/// this light
/// Based on formula: attenuation = light.brightness / (distance² / LIGHT_DIST)
/// Light contributions below 1/512 are considered negligible
float	max_light_distance_sq(t_light light)
{
	static const float	threshold = 0.0019;
	float				max_color_component;
	float				effective_brightness;
	float				max_distance_squared;

	max_color_component = fmaxf(light.color.x,
			fmaxf(light.color.y, light.color.z));
	effective_brightness = light.brightness * max_color_component;
	max_distance_squared = effective_brightness * LIGHT_DIST / threshold;
	return (max_distance_squared);
}

void	set_lights_shining_dist(t_light *lights, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		lights[i].max_dist = max_light_distance_sq(lights[i]);
		i++;
	}
}

static bool	lights_for_aabb(t_scene *scene, t_aabb aabb, t_light ***out)
{
	t_light	*lights;
	t_vec3	closest;
	size_t	i;
	size_t	j;

	lights = scene->lights;
	*out = malloc(sizeof(t_light *) * (scene->light_count + 1));
	if (!*out)
		return (false);
	i = 0;
	j = 0;
	while (i < scene->light_count)
	{
		closest = aabb_closest_point(aabb, lights[i].pos);
		if (vec3_squared_length(vec3_subtract(closest, lights[i].pos))
			< lights[i].max_dist)
			(*out)[j++] = &lights[i];
		i++;
	}
	(*out)[j] = NULL;
	return (true);
}

bool	malloc_light_objects(t_scene *scene)
{
	size_t	i;

	i = 0;
	while (i < scene->light_count)
	{
		scene->lights[i].objects.sphere_count = scene->objects.sphere_count;
		scene->lights[i].objects.cylinder_count = scene->objects.cylinder_count;
		scene->lights[i].objects.cone_count = scene->objects.cone_count;
		scene->lights[i].objects.plane_count = scene->objects.plane_count;
		if (!ft_malloc_objects(&scene->lights[i].objects))
			return (false);
		scene->lights[i].objects.sphere_count = 0;
		scene->lights[i].objects.cylinder_count = 0;
		scene->lights[i].objects.cone_count = 0;
		scene->lights[i].objects.plane_count = 0;
		i++;
	}
	i = 0;
	return (true);
}

bool	fill_objects_for_lights(t_scene *scene)
{
	ssize_t	i;
	ssize_t	j;

	i = -1;
	while (++i < (ssize_t)scene->objects.sphere_count)
	{
		j = -1;
		while (scene->objects.spheres[i].lights[++j])
		{
			scene->objects.spheres[i].lights[j]->objects. \
				spheres[scene->objects.spheres[i].lights[j] \
					->objects.sphere_count++] = scene->objects.spheres[i];
		}
	}
	i = -1;
	while (++i < (ssize_t)scene->objects.cylinder_count)
	{
		j = -1;
		while (scene->objects.cylinders[i].lights[++j])
		{
			scene->objects.cylinders[i].lights[j]->objects. \
				cylinders[scene->objects.cylinders[i].lights[j] \
					->objects.cylinder_count++] = scene->objects.cylinders[i];
		}
	}
	i = -1;
	while (++i < (ssize_t)scene->objects.cone_count)
	{
		j = -1;
		while (scene->objects.cones[i].lights[++j])
		{
			scene->objects.cones[i].lights[j]->objects. \
				cones[scene->objects.cones[i].lights[j] \
					->objects.cone_count++] = scene->objects.cones[i];
		}
	}
	return (true);
}

bool	fill_lights_for_objects(t_scene *scene)
{
	ssize_t	i;

	i = -1;
	while (++i < (ssize_t)scene->objects.sphere_count)
	{
		if (!lights_for_aabb(scene, sphere_aabb(&scene->objects.spheres[i]),
				&scene->objects.spheres[i].lights))
			return (false);
	}
	i = -1;
	while (++i < (ssize_t)scene->objects.cylinder_count)
	{
		if (!lights_for_aabb(scene, cylinder_aabb(&scene->objects.cylinders[i]),
				&scene->objects.cylinders[i].lights))
			return (false);
	}
	i = -1;
	while (++i < (ssize_t)scene->objects.cone_count)
	{
		if (!lights_for_aabb(scene, cone_aabb(&scene->objects.cones[i]),
				&scene->objects.cones[i].lights))
			return (false);
	}
	return (true);
}
