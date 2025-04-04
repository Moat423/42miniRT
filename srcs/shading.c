/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/28 12:55:30 by lmeubrin         ###   ########.fr       */
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

static bool	is_in_shadow(t_scene *scene, t_ray ray)
{
	t_intersection	intersection;

	return (find_closest_intersection(scene, ray, &intersection));
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
			if (!is_in_shadow(scene, (t_ray){intersection.point, 
					l.direction, interval_new(EPSILON, l.distance)}))
				color = vec3_add(color, 
						calc_lights(scene->lights[i], ray, intersection, l));
		}
		i++;
	}
	return (color);
}
