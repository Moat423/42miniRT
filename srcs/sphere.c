/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 14:24:04 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	sphere_normal(t_sphere sphere, t_vec3 point)
{
	return (vec3_normalize(vec3_subtract(point, sphere.pos)));
}

t_intersection	sphere_intersect(t_sphere *sphere, t_ray ray)
{
	t_vec3			oc;
	t_intersection	intersection;
	float			ahc[3];
	float			discriminant;
	float			t;

	oc = vec3_subtract(ray.origin, sphere->pos);
	ahc[0] = vec3_squared_length(ray.direction);
	ahc[1] = vec3_dot(oc, ray.direction);
	ahc[2] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	discriminant = ahc[1] * ahc[1] - ahc[0] * ahc[2];
	if (discriminant < 0)
		return ((t_intersection){0});
	t = (ahc[1] - sqrt(discriminant)) / ahc[0];
	if (t < 0)
		return ((t_intersection){0});
	intersection = (t_intersection){
		.distance = t,
		.point = vec3_add(ray.origin, vec3_multiply(ray.direction, t)),
		.object = {
			.sphere = sphere,
			.type = SPHERE,
		},
	};
	return (intersection);
}
