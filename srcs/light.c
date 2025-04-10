/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:07:42 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/31 13:35:54 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/*
* blinn phong using half vector
*  Specular = Ks * Is * (max(0, N·H))^shininess
*  where H = normalize(L + V)
*/
// static t_color	blinn_phong(const t_vec3 half_dir, const t_vec3 hit_normal,
// 							const float l_attenuation, const t_vec3 l_color)
// {
// 	t_color	spec_color;
// 	t_color	mixed_color;
// 	float	specular;
//
// 	mixed_color = vec3_component_mul(MATERIAL_COLOR, l_color);
// 	specular = powi(fmaxf(vec3_dot(hit_normal, half_dir), 0.0f), 128);
// 	spec_color = vec3_multiply(mixed_color, (specular * l_attenuation));
// 	return (spec_color);
// }

/*
*orignial Phong
*Specular = Ks * Is * (max(0, R·V))^shininess
*where R = 2*(N·L)*N - L
*/
static	t_color	original_phong(const t_vec3 view_dir, const t_vec3 hit_normal,
							  const float l_attenuation, const t_color l_color, t_vec3 light_dir)
{
	t_color	mixed_color;
	float	specular;
	t_color	spec_color;

    float n_dot_l = vec3_dot(hit_normal, light_dir);
    t_vec3 reflection = vec3_subtract(
        vec3_multiply(hit_normal, 2.0f * n_dot_l),
        light_dir
    );
	mixed_color = vec3_component_mul(MATERIAL_COLOR, l_color);
	specular = powi(fmaxf(vec3_dot(reflection, view_dir), 0.0f), SHININESS);
	spec_color = vec3_multiply(mixed_color, (specular * l_attenuation));
	return (spec_color);
}

// Diffuse = Kd * Id * max(0, N·L)
// Specular = Ks * Is * max(0, R·V)^shininess
t_color	calc_lights(const t_light light, const t_ray ray, t_intersection its, t_light_ray l)
{
	t_color	diffuse;
	t_color	spec_color;

	l.attenuation = light.brightness / (l.distance * l.distance / LIGHT_DIST);
	diffuse = vec3_component_mul(object_color(its.object), light.color);
	diffuse = vec3_multiply(object_color(its.object), l.lambert * l.attenuation);

	// spec_color = blinn_phong(vec3_normalize(
	// 		vec3_add(l.direction, vec3_multiply(ray.direction, -1.0))),
	// 		its.normal, l.attenuation, light.color);

	spec_color = original_phong(
			vec3_multiply(ray.direction, -1.0),
			its.normal, l.attenuation, light.color,
			l.direction);

	return (vec3_add(diffuse, spec_color));
}
