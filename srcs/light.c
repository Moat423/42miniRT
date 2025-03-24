/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:07:42 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/24 13:07:56 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

			// color = specular(scene->lights[i], light_dir, normal, color, lambert, light_dist);
t_color	specular(t_light light, t_vec3 light_dir, t_vec3 normal, t_vec3 diffuse_color, float lambertian, float distance)
{
	t_vec3	view_dir;
	t_vec3	half_dir;
	float	specular_angle;
	float	specular;
	t_vec3	specular_color;
	t_vec3	res;
	float	light_diminish;

	specular_color = color_new(1, 1, 1);

	view_dir = vec3_multiply(light_dir, -1);
	half_dir = vec3_normalize(vec3_add(light_dir, view_dir));
	specular_angle = fmaxf(vec3_dot(half_dir, normal), 0.0);
	specular = specular_angle * specular_angle * specular_angle * specular_angle * specular_angle;
	light_diminish = light.brightness / distance;
	res = vec3_add (vec3_multiply((vec3_component_mul(vec3_multiply(diffuse_color, lambertian),  light.color)),  light_diminish), \
	 (vec3_multiply(vec3_component_mul(vec3_multiply(specular_color, specular),  light.color),  light_diminish)));
	return (res);
}

			////specular
			//// color = vec3_add(color, specular((scene->lights[i]), ray, intersection, light_dir, normal, color));
			//t_vec3 refl_dir = vec3_subtract(vec3_multiply(vec3_multiply(normal, 0.2),  vec3_dot(normal, light_dir)),light_dir);
			//             float r_dot_v = vec3_dot(refl_dir, ray.direction);
			//         if (r_dot_v > 0)
			//                 i += scene->lights[i].brightness * vec3_power(r_dot_v/vec3_multiply(vec3_squared_length(refl_dir), vec3_squared_length(ray.direction)), 10)
