/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/9 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/24 19:15:09 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	set_intersect_normal(t_sphere *sphere, t_intersection *intersection)
{
	intersection->normal = vec3_divide(vec3_subtract(intersection->point, sphere->pos), sphere->radius);
	intersection->normal_calculated = true;
}

bool	sphere_intersect(t_sphere *sphere, t_ray ray, t_intersection *out)
{
	t_vec3			oc;
	float			abc[3];
	float			discriminant;
	float			sqrt_d;
	float			t[2];

	// P = O + t * D
	// (P - C)² = R²
	// (O + t * D - C)² = R²
	// (O-C + t * D)² = R²
	// D^2*t^2 + 2OC tD + (OC)^2 - R^2 = 0
	oc = vec3_subtract(ray.origin, sphere->pos);
	// a = 1 (|normal vector|² = 1) = D²
	// b = 2 * oc * D
	abc[B] = 2 * vec3_dot(oc, ray.direction);
	// c = oc² - r²
	abc[C] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	// disc = b*b - 4*1*c
	discriminant = abc[B] * abc[B] - 4 * abc[C];
	if (discriminant < 0)
		return (false);
	if (discriminant == 1.0)
	{
		// -b / 2*1
		t[0] = -abc[B] * 0.5;
		t[1] = t[0];
	}
	else
	{
		sqrt_d = sqrtf(discriminant);
		// (-b +- sqrt(disc)) / 2*1
		t[0] = (-abc[B] - sqrt_d) * 0.5;
		t[1] = (-abc[B] + sqrt_d) * 0.5;
	}
	// we want the closest intersection point thats not behind the camera/ ray origin
	if (t[0] < ray.range.min)
		t[0] = t[1];
	// if there is none we dont have an intersection in the ray's direction
	if (t[0] < ray.range.min)
		return (false);
	out->distance = t[0];
	if (!interval_contains(ray.range, out->distance))
		return (false);
	// P = O + t * D
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	set_intersect_normal(sphere, out);
	return (true);
}
