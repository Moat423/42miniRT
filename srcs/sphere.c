/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/20 15:54:40 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	sphere_normal(t_sphere sphere, t_vec3 point)
{
	return (vec3_divide(vec3_subtract(point, sphere.pos), sphere.radius));
}

bool	sphere_intersect(t_sphere *sphere, t_ray ray, t_intersection *out)
{
	t_vec3			oc;
	float			abc[3];
	float			discriminant;
	float			sqrt_d;
	float			t;

	(void)t;
	// L
	oc = vec3_subtract(sphere->pos, ray.origin);
	// a
	abc[0] = vec3_squared_length(ray.direction);
	// b
	abc[1] = 2 * vec3_dot(oc, ray.direction);
	// c
	abc[2] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	discriminant = abc[1] * abc[1] - 4 * abc[0] * abc[2]; // b*b - 4*a*c
	if (discriminant < 0)
		return (false);
	sqrt_d = sqrt(discriminant);
	float q = (abc[1] > 0) ?
            -0.5 * (abc[1] + sqrt_d) :
            -0.5 * (abc[1] - sqrt_d);
	float x0 = q / abc[0];
	float x1 = abc[2] / q;
	if (x0 > x1)
	{
		float tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	out->distance = (x0 < 0) ? x1 : x0;
	// ray.orig + ray.dir * t
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	// -0.5 * (b - sqrt(disc)) / a
	//t = -0.5 * (abc[1] - sqrt_d) / abc[0];
	//if (t < 0)
	//	t = (abc[1] + sqrt_d) / abc[0];
	//if (t < 0)
	//	return (false);
	//out->distance = t;
	//out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
	//out->object = (t_object){.sphere = sphere, .type = SPHERE};
	return (true);
}
