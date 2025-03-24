/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/24 16:40:28 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include <stdbool.h>

static float	lambertian(t_vec3 normal, t_vec3 light_dir)
{
	return (fmaxf(0.0f, vec3_dot(normal, light_dir)));
}

static t_color	light_diffuse(t_light light, t_object object, float lambertian, float light_dist_sq)
{
	t_color	color;

	(void)light_dist_sq;
	color = vec3_component_mul(object_color(object), light.color);
	color = vec3_multiply(color, lambertian * light.brightness / (light_dist_sq / 5.0));
	return (color);
}

static t_color	ambient(t_scene *scene, t_object object)
{
	return (vec3_multiply(
		vec3_component_mul(scene->ambient.color, object_color(object)),
		scene->ambient.brightness));
}

static bool	is_in_shadow(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;

	return (find_closest_intersection(scene, ray, &intersection));
}

t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection)
{
	t_color	color;
	t_vec3	light_dir;
	t_vec3	normal;
	float	light_dist;
	size_t	i;

	(void)ray;
	color = ambient(scene, intersection.object);
	// dont calculate the normal multiple times for the same object but rather include it in intersection struct
	normal = object_normal(intersection.object, intersection.point);
	i = 0;
	(void)light_dir;
	(void)light_dist;
	while (i < scene->light_count)
	{
		light_dir = vec3_calc_length_and_normalize(
			vec3_subtract(scene->lights[i].pos, intersection.point),
			&light_dist);
		// if (lambert > 0)
		// {
		if (!is_in_shadow(scene, (t_ray){.origin=intersection.point, .direction=light_dir, .range=interval_new(EPSILON, light_dist)}))
		{
			color = vec3_add(color,
				light_diffuse(scene->lights[i],
					intersection.object,
					lambertian(normal, light_dir),
					light_dist * light_dist)
				);
		}
		i++;
	}
	return (color);
}
