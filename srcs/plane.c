/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:51:03 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/24 16:09:07 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

bool	plane_intersect(t_plane *plane, t_ray ray, t_intersection *out, t_interval ray_t)
{
	float	res;
	// t_vec3			oc;
	float	t;
	float	denom;

	// oc = vec3_subtract(ray.origin, plane->pos);
	denom =  vec3_dot(plane->normal, ray.direction);
	// plane might have a normal looking into wrong dir
	// but we can't detect if camera looks into wrong dir
	if (denom > EPSILON || denom < -EPSILON)
	{
		t = vec3_dot(vec3_subtract(plane->pos, ray.direction), plane->normal) / denom;
		if (!interval_contains(ray_t, t))
			return (false);
		out->distance = t;
		out->object.plane = plane;
		out->object.type = PLANE;
		out->point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
		return (true);
	}
	return (false);
}
