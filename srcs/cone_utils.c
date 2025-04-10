/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:23:10 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 11:52:02 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// gets discriminant of cone
// the axis is normalized and therefore the normal of the cone
// coeff: 1 + slant * slant
// a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
// b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
// c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
float	cone_get_discriminant(const t_vec3 ray_dir, const t_calc cc,
							float abc[3])
{
	abc[A] = vec3_squared_length(ray_dir) - cc.coeff * cc.d_dot_n * cc.d_dot_n;
	abc[C] = vec3_squared_length(cc.oc) - cc.coeff * cc.oc_dot_n * cc.oc_dot_n;
	abc[B] = 2 * (vec3_dot(ray_dir, cc.oc) 
			- cc.coeff * cc.d_dot_n * cc.oc_dot_n);
	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
}

t_calc	prep_cone_calc(const t_ray ray, const t_cone *cone)
{
	t_calc	cc;

	cc.oc = vec3_subtract(ray.origin, cone->top);
	cc.d_dot_n = vec3_dot(ray.direction, cone->axis);
	cc.oc_dot_n = vec3_dot(cc.oc, cone->axis);
	cc.coeff = 1 + cone->slant * cone->slant;
	cc.height = cone->height;
	return (cc);
}

bool	cone_calc(const t_cone *cone, const t_ray ray,
						float t[2], t_calc *cc)
{
	float		abc[3];
	float		discriminant;
	float		sqrt_d;

	*cc = prep_cone_calc(ray, cone);
	discriminant = cone_get_discriminant(ray.direction, *cc, abc);
	if (discriminant < EPSILON || (fabs(abc[C]) < EPSILON))
		return (false);
	if (fabs(abc[A]) < EPSILON)
	{
		if (fabs(abc[B]) < EPSILON)
			return (false);
		t[0] = -abc[C] / abc[B];
		t[1] = t[0];
		return (true);
	}
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	return (true);
}
