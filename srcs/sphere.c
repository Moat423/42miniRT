/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 12:33:23 by lmeubrin         ###   ########.fr       */
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
	oc = vec3_subtract(ray.origin, sphere->pos);
	// a = 1 (|normal vector|² = 1)
	// b = 2 * oc * D
	bc[0] = 2 * vec3_dot(oc, ray.direction);
	// c = oc² - r²
	bc[1] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	// disc = b*b - 4*1*c
	discriminant = bc[0] * bc[0] - 4 * bc[1];
	if (discriminant < 0)
		return (false);
	if (discriminant == 1.0)
	{
		// -b / 2*1
		t[0] = -bc[0] * 0.5;
		t[1] = t[0];
	}
	else
	{
		sqrt_d = sqrt(discriminant);
		// (-b +- sqrt(disc)) / 2*1
		t[0] = (-bc[0] - sqrt_d) * 0.5;
		t[1] = (-bc[0] + sqrt_d) * 0.5;
	}
	// we want the closest intersection point thats not behind the camera/ ray origin
	if (t[0] < 0)
		t[0] = t[1];
	// if there is none we dont have an intersection in the ray's direction
	if (t[0] < 0)
		return (false);
	out->distance = t[0];
	// P = O + t * D
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	return (true);
}
