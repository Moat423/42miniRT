/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/10 14:00:32 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include <stdbool.h>

static float	lambertian(t_vec3 normal, t_vec3 light_dir)
{
	return (fmaxf(0.0f, vec3_dot(normal, light_dir)));
}

// OBSOLETE FUNCTION RIGHT HERE!!!!!!!!!!!
// // Diffuse = Kd * Id * max(0, N·L)
// static t_color	light_diffuse(t_light light, t_object object, float lambertian,
// 							float light_dist_sq)
// {
// 	t_color	color;
//
// 	color = vec3_component_mul(object_color(object), light.color);
// 	color = vec3_multiply(color, lambertian *
// 			light.brightness / (light_dist_sq / 5.0));
// 	return (color);
// }

static t_color	ambient(t_scene *scene, t_object object)
{
	return (vec3_multiply(
			vec3_component_mul(scene->ambient.color, object_color(object)),
			scene->ambient.brightness));
}

bool	shadow_iterate_over_obj((bool)hit_anywhere(object, ray), (void *)(char *)objects, size_t count)
{
	size_t		i;

	i = 0;
	while (i < count)
	{
		if (hit_anywhere((void *)((char *)objects + i * elem_size), ray, &intersection))
			return (true);
		i++;
	}
}

static bool	is_in_shadow(t_scene *scene, t_ray ray)
{
	// t_intersection	intersection;
	//
	// return (find_closest_intersection(scene, ray, &intersection));
	if (shadow_iterate_over_obj(plane_hit_anywhere(), scene->planes, scene->plane_count))
		return (true);
	if (shadow_iterate_over_obj(circle_hit_anywhere(), scene->spheres, scene->sphere_count))
		return (true);
}

t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection)
{
	t_color		color;
	t_vec3		normal;
	size_t		i;
	t_light_ray	l;

	(void) ray;
	color = ambient(scene, intersection.object);
	normal = intersect_normal(&intersection);
	i = 0;
	while (i < scene->light_count)
	{
		l.direction = vec3_calc_length_and_normalize(
				vec3_subtract(scene->lights[i].pos, intersection.point),
				&(l.distance));
		l.lambert = lambertian(normal, l.direction);
		if (l.lambert > 0)
		{
			if (!is_in_shadow(scene, (t_ray){vec3_add(intersection.point, vec3_multiply(intersect_normal(&intersection), EPSILON)),
					l.direction, interval_new(EPSILON, l.distance)}))
				color = vec3_add(color,
						calc_lights(scene->lights[i], ray, intersection, l));
		}
		i++;
	}
	return (color);
}
