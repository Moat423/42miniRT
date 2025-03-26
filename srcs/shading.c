/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:40:23 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/26 17:06:48 by lmeubrin         ###   ########.fr       */
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

t_color	calc_lights(const t_light light, const t_ray ray, t_intersection ints)

 t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection)
 {
 	t_color	color;
 	t_vec3	normal;
 	float	lambert;
 	t_vec3	diff_spec_color;
 	t_vec3	light_dir;
 	float	light_dist;
 	float	light_attenuation;
 	size_t	i;
 	t_color	spec_color;
 	t_vec3	half_dir;
 	float	specular;

 	color = ambient(scene, intersection.object);
 	normal = intersect_normal(&intersection);
 	i = 0;
 	while (i < scene->light_count)
 	{
 		light_dir = vec3_calc_length_and_normalize(
 			vec3_subtract(scene->lights[i].pos, intersection.point),
 			&light_dist);
 		lambert = lambertian(normal, light_dir);
 		if (lambert <= 0)
			continue ;
		if (!is_in_shadow(scene, (t_ray){.origin=intersection.point, .direction=light_dir, .range=interval_new(EPSILON, light_dist)}))
			diff_spec_color = calc_lights(scene->lights[i], ray, intersection);
	}
 }


t_color	shade(t_scene *scene, t_ray ray, t_intersection intersection)
{
	t_color	color;
	t_vec3	light_dir;
	t_vec3	normal;
	float	light_dist;
	size_t	i;
	float	lambert;
	t_vec3	diffuse_color;
	t_color	spec_color;
	t_vec3	half_dir;
	float	light_diminish;
	float	specular;

	(void)ray;
	color = ambient(scene, intersection.object);
	normal = intersect_normal(&intersection);
	i = 0;
	while (i < scene->light_count)
	{
		light_dir = vec3_calc_length_and_normalize(
			vec3_subtract(scene->lights[i].pos, intersection.point),
			&light_dist);
		lambert = lambertian(normal, light_dir);
		if (lambert > 0)
		{
			if (!is_in_shadow(scene, (t_ray){.origin=intersection.point, .direction=light_dir, .range=interval_new(EPSILON, light_dist)}))
			{
				diffuse_color = 
					light_diffuse(scene->lights[i],
						intersection.object,
						lambert,
						light_dist * light_dist)
					;
				light_diminish = scene->lights[i].brightness / (light_dist / 5.0);
				half_dir = vec3_normalize(vec3_add(light_dir, vec3_multiply(ray.direction, -1.0)));
				specular = fmax((powi(vec3_dot(normal, half_dir), 80) * light_diminish), 0);
				spec_color = vec3_multiply((t_vec3){0.9, 0.9, 0.9}, specular);
				color = vec3_add(color, diffuse_color);
				color = vec3_add(color, spec_color);
			}
		}
		i++;
	}
	return (color);
}
