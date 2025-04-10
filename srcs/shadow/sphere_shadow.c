/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_shadow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:00:42 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 12:23:15 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

bool	sphere_solve_quadr_eq(float discriminant, const float abc[3],
							const t_interval range)
{
	float			t[2];

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
	if (t[0] < range.min)
		t[0] = t[1];
	if (!interval_contains(range, t[0]))
		return (false);
	return (true);
}

// previously:
	// if (discriminant < 0)
	// 	return (false);
	// if (discriminant == 1.0)
	// {
	// 	t[0] = -abc[B] * 0.5;
	// 	t[1] = t[0];
	// }
	// else
	// {
	// 	sqrt_d = sqrtf(discriminant);
	// 	t[0] = (-abc[B] - sqrt_d) * 0.5;
	// 	t[1] = (-abc[B] + sqrt_d) * 0.5;
	// }
	// if (t[0] < ray.range.min)
	// 	t[0] = t[1];
	// if (!interval_contains(ray.range, t[0]))
	// 	return (false);
	// return (true);
bool	sphere_hit_anywhere(t_sphere *sphere, t_ray ray)
{
	t_vec3			oc;
	float			abc[3];
	float			discriminant;
	float			sqrt_d;
	float			t[2];

	oc = vec3_subtract(ray.origin, sphere->pos);
	abc[B] = 2 * vec3_dot(oc, ray.direction);
	abc[C] = vec3_squared_length(oc) - sphere->radius * sphere->radius;
	discriminant = abc[B] * abc[B] - 4 * abc[C];
	return (sphere_solve_quadr_eq(discriminant, abc, ray.range));
}
