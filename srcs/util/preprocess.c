/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:38:14 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 14:49:52 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"
#include <stdbool.h>

int	ft_malloc_objects(t_objects *objects);

/// Returns the distance² after which there is no light contribution from
/// this light
/// Based on formula: attenuation = light.brightness / (distance² / LIGHT_DIST)
/// Light contributions below 1/512=0.0019 are considered negligible
float	max_light_distance_sq(t_light light)
{
	static const float	threshold = 0.005;
	float				max_color_component;
	float				effective_brightness;
	float				max_distance_squared;

	max_color_component = fmaxf(light.color.x,
			fmaxf(light.color.y, light.color.z));
	effective_brightness = light.brightness * max_color_component;
	max_distance_squared = effective_brightness * LIGHT_DIST / threshold;
	return (max_distance_squared);
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
			< max_light_distance_sq(lights[i]))
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
		scene->lights[i].objs.sphere_count = scene->objs.sphere_count;
		scene->lights[i].objs.cylinder_count = scene->objs.cylinder_count;
		scene->lights[i].objs.cone_count = scene->objs.cone_count;
		scene->lights[i].objs.plane_count = scene->objs.plane_count;
		if (!ft_malloc_objects(&scene->lights[i].objs))
			return (false);
		scene->lights[i].objs.sphere_count = 0;
		scene->lights[i].objs.cylinder_count = 0;
		scene->lights[i].objs.cone_count = 0;
		scene->lights[i].objs.plane_count = 0;
		i++;
	}
	i = 0;
	return (true);
}

bool	fill_lights_for_objects(t_scene *scene)
{
	ssize_t	i;

	i = -1;
	while (++i < (ssize_t)scene->objs.sphere_count)
	{
		if (!lights_for_aabb(scene, sphere_aabb(&scene->objs.spheres[i]),
				&scene->objs.spheres[i].lights))
			return (false);
	}
	i = -1;
	while (++i < (ssize_t)scene->objs.cylinder_count)
	{
		if (!lights_for_aabb(scene, cylinder_aabb(&scene->objs.cylinders[i]),
				&scene->objs.cylinders[i].lights))
			return (false);
	}
	i = -1;
	while (++i < (ssize_t)scene->objs.cone_count)
	{
		if (!lights_for_aabb(scene, cone_aabb(&scene->objs.cones[i]),
				&scene->objs.cones[i].lights))
			return (false);
	}
	return (true);
}
