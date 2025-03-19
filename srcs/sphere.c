/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:01:02 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 14:45:13 by kwurster         ###   ########.fr       */
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
	float			ahc[3];
	float			discriminant;
	float			sqrt_d;
	float			t;

	oc = vec3_subtract(ray.origin, sphere->pos);
	ahc[0] = vec3_squared_length(ray.direction);
	ahc[1] = vec3_dot(oc, ray.direction);
	ahc[2] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	discriminant = ahc[1] * ahc[1] - ahc[0] * ahc[2];
	if (discriminant < 0)
		return (false);
	sqrt_d = sqrt(discriminant);
	t = (ahc[1] - sqrt_d) / ahc[0];
	if (t < 0)
		t = (ahc[1] + sqrt_d) / ahc[0];
	if (t < 0)
		return (false);
	out->distance = t;
	out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
	out->object = (t_object){.sphere = sphere, .type = SPHERE};
	return (true);
}
