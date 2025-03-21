/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 15:00:36 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static float	lambertian(t_vec3 normal, t_vec3 light_dir)
{
	return (fmaxf(0.0f, vec3_dot(normal, light_dir)));
}

static t_color	light_diffuse(t_light light, t_object object, float lambertian, float light_dist_sq)
{
	t_color	color;
	color = vec3_component_mul(object_color(object), light.color);
	color = vec3_multiply(color, lambertian * light.brightness);
	(void)light_dist_sq;
	// / light_dist_sq
	return (color);
}

static t_color	ambient(t_scene *scene, t_object object)
{
	return (vec3_multiply(
		vec3_component_mul(scene->ambient.color, object_color(object)),
		scene->ambient.brightness));
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
		// TODO check if light is blocked by another object
		// if (!in_shadow(...))
		// use find_closest_intersection with the light_dir and the light point
		// find_closest_intersection must be given a new parameter, the interval
		// which in this case must be from 0 to (light_dist - 0.001) to not intersect with our own object
		// https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/anintervalclass
		color = vec3_add(color,
			light_diffuse(scene->lights[i],
				intersection.object,
				lambertian(normal, light_dir),
				light_dist * light_dist)
			);
		i++;
	}
	return (color);
}
