/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/9 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/16 16:48:08 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	set_intersect_normal(t_sphere *sphere, t_intersection *intersection)
{
	intersection->normal = vec3_divide(
			vec3_subtract(intersection->point, sphere->pos), sphere->radius);
	intersection->normal_calculated = true;
}

bool	check_hit(float t[2], t_ray ray, t_intersection *out, t_sphere *sphere)
{
	if (t[0] < ray.range.min)
		t[0] = t[1];
	if (t[0] < ray.range.min)
		return (false);
	out->distance = t[0];
	if (!interval_contains(ray.range, out->distance))
		return (false);
	out->point = vec3_add(ray.origin,
			vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	set_intersect_normal(sphere, out);
	return (true);
}

bool	sphere_intersect(t_sphere *sphere, t_ray ray, t_intersection *out)
{
	t_vec3			oc;
	float			abc[3];
	float			discriminant;
	float			t[2];

	oc = vec3_subtract(ray.origin, sphere->pos);
	abc[B] = 2 * vec3_dot(oc, ray.direction);
	abc[C] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	discriminant = abc[B] * abc[B] - 4 * abc[C];
	if (discriminant < 0)
		return (false);
	if (discriminant == 1.0)
	{
		t[0] = -abc[B] * 0.5;
		t[1] = t[0];
	}
	else
	{
		discriminant = sqrtf(discriminant);
		t[0] = (-abc[B] - discriminant) * 0.5;
		t[1] = (-abc[B] + discriminant) * 0.5;
	}
	if (!check_hit(t, ray, out, sphere))
		return (false);
	return (true);
}
