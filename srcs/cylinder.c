/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/24 19:42:53 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
{
	t_vec3	oc;
	float	abc[3];
	float	discriminant;
	float	sqrt_d;
	float	t[2];

	oc = vec3_subtract(ray.origin, cylinder->pos);
	abc[A] = vec3_squared_length(ray.direction) - vec3_dot(ray.direction, cylinder->axis) * vec3_dot(ray.direction, cylinder->axis);
	abc[C] = vec3_squared_length(oc) - (vec3_dot(oc, cylinder->axis) * vec3_dot(oc, cylinder->axis)) - cylinder->radius * cylinder->radius;
	abc[B] = 2 * vec3_dot(ray.direction, oc) - vec3_dot(ray.direction, cylinder->axis) * vec3_dot(oc, cylinder->axis);
	discriminant = abc[B] * abc[B] - 4 * abc[A] * abc[C];
	if (discriminant < 0)
		return (false);
	// if (discriminant == 1.0)
	// {
	// 	// -b / 2*1
	// 	t[0] = -abc[B] / (2 * abc[A]);
	// 	t[1] = t[0];
	// 	printf("one solution");
	// }
	// else
	// {
	sqrt_d = sqrtf(discriminant);
	// (-b +- sqrt(disc)) / 2*1
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	// }
	// we want the closest intersection point thats not behind the camera/ ray origin
	if (t[0] < ray.range.min)
		t[0] = t[1];
	// if there is none we dont have an intersection in the ray's direction
	if (t[0] < ray.range.min)
		return (false);
	out->distance = t[0];
	// if (!interval_contains((t_interval), out->distance))
	if (!interval_contains(ray.range, out->distance))
		return (false);
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	return (true);
}
