/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:28:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/31 13:27:05 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// gets discriminant of cone
// the axis is normalized and therefore the normal of the cone
// coeff: 1 + slant * slant
// a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
// b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
// c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
static	float	get_discriminant(float abc[3], const t_ray ray, 
								const t_cone *cone)
{
	t_vec3	oc;
	float	d_dot_ax;
	float	oc_dot_ax;
	float	coeff;

	oc = vec3_subtract(ray.origin, cone->top);
	d_dot_ax = vec3_dot(ray.direction, cone->axis);
	oc_dot_ax = vec3_dot(oc, cone->axis);
	coeff = 1 + cone->slant * cone->slant;
	abc[A] = vec3_squared_length(ray.direction) - coeff * d_dot_ax * d_dot_ax;
	abc[C] = vec3_squared_length(oc) - coeff * oc_dot_ax * oc_dot_ax;
	abc[B] = 2 * (vec3_dot(ray.direction, oc) - coeff * d_dot_ax * oc_dot_ax);
	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
}

