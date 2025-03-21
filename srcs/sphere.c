/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/9 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 12:44:22 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	sphere_normal(t_sphere sphere, t_vec3 point)
{
	t_vec3	normal;

	normal = vec3_divide(vec3_subtract(point, sphere.pos), sphere.radius);
	return (normal);
}

bool	sphere_intersect(t_sphere *sphere, t_ray ray, t_intersection *out)
{
	t_vec3			oc;
	float			bc[2];
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
	bc[B] = 2 * vec3_dot(oc, ray.direction);
	// c = oc² - r²
	bc[C] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	// disc = b*b - 4*1*c
	discriminant = bc[B] * bc[B] - 4 * bc[C];
	if (discriminant < 0)
		return (false);
	if (discriminant == 1.0)
	{
		// -b / 2*1
		t[B] = -bc[B] * 0.5;
		t[C] = t[B];
	}
	else
	{
		sqrt_d = sqrtf(discriminant);
		// (-b +- sqrt(disc)) / 2*1
		t[B] = (-bc[B] - sqrt_d) * 0.5;
		t[C] = (-bc[B] + sqrt_d) * 0.5;
	}
	// we want the closest intersection point thats not behind the camera/ ray origin
	if (t[B] < 0)
		t[B] = t[C];
	// if there is none we dont have an intersection in the ray's direction
	if (t[B] < 0)
		return (false);
	out->distance = t[B];
	// P = O + t * D
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	return (true);
}
