/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:51:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/21 14:39:06 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// geturn an intersection with the plane if there is one
//
// to enable bumpmaps use:
// out->normal = plane_apply_bump_mapping(plane, out->point,
// 		out->normal);
bool	plane_intersect(t_plane *plane, t_ray ray, t_intersection *out)
{
	float	t;
	float	denom;

	denom = vec3_dot(plane->normal, ray.direction);
	if (denom > EPSILON || denom < -EPSILON)
	{
		t = vec3_dot(vec3_subtract(plane->pos, ray.origin),
				plane->normal) / denom;
		if (!interval_contains(ray.range, t))
			return (false);
		out->distance = t;
		out->object.plane = plane;
		out->object.type = PLANE;
		out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
		out->normal = plane->normal;
		out->normal_calculated = true;
		return (true);
	}
	return (false);
}
