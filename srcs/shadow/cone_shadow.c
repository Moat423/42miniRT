/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_shadow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:47:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 15:12:54 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static bool	cone_body_hit_anywhere(t_calc cc, const t_ray ray, float *t)
{
	t_vec3		hit_point;
	float		hit_proj;
	int			i;

	i = -1;
	while (++i < 2)
	{
		if (interval_contains(ray.range, t[i]))
		{
			hit_point = vec3_add(ray.origin, 
					vec3_multiply(ray.direction, t[i]));
			hit_proj = cc.d_dot_n * t[i] + cc.oc_dot_n;
			if (hit_proj >= 0 && hit_proj <= cc.height)
				return (true);
		}
	}
	return (false);
}

bool	cone_hit_anywhere(t_cone *cone, const t_ray ray)
{
	t_calc		cc;
	float		t[2];

	if (!cone_calc(cone, ray, t, &cc))
		return (circle_hit_anywhere((t_circle){cone->bottom, cone->axis,
				cone->radius}, ray));
	if (cone_body_hit_anywhere(cc, ray, t))
		return (true);
	return (circle_hit_anywhere((t_circle){cone->bottom, cone->axis,
			cone->radius}, ray));
}
