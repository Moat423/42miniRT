/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 12:29:16 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_intersection	sphere_intersect(t_sphere *sphere, t_ray ray)
{
	t_vec3			oc;
	t_intersection	intersection;
	float			abc[3];
	float			discriminant;
	float			t;

	oc = vec3_subtract(ray.origin, sphere->pos);
	abc[0] = vec3_dot(ray.direction, ray.direction);
	abc[1] = -2 * vec3_dot(oc, ray.direction);
	abc[2] = vec3_dot(oc, oc) - (sphere->diameter/2 * sphere->diameter/2);
	discriminant = abc[1] * abc[1] - 4 * abc[0] * abc[2];
	if (discriminant < 0)
		return ((t_intersection){0});
	t = (-abc[1] - sqrt(discriminant)) / (2.0*abc[0]);
	if (t < 0)
		return ((t_intersection){0});
	intersection = (t_intersection){
		.distance = t,
		.point = vec3_add(ray.origin, vec3_multiply(ray.direction, t)),
		.object = sphere,
		.type = SPHERE,
	};
	intersection.normal = vec3_normalize(vec3_subtract(intersection.point, sphere->pos));
	return (intersection);
}
