/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_shadow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:03:52 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 12:05:36 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

bool	plane_hit_anywhere(t_plane *plane, t_ray ray)
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
		return (true);
	}
	return (false);
}
