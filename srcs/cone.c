/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:28:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 11:52:24 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Calculate normal at hit point
static t_vec3	calc_cone_normal(const t_vec3 hit_point, const t_vec3 top,
									const t_vec3 axis, const float m)
{
	t_vec3	axis_point;

	axis_point = vec3_add(top, vec3_multiply(axis, m));
	return (vec3_normalize(vec3_subtract(hit_point, axis_point)));
}

//returns the hit projection along the normal
//or -1 if none was found
static float	cone_body_hit(t_calc cc, const t_ray ray,
							t_intersection *out, float *t)
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
			{
				out->distance = t[i];
				out->point = hit_point;
				return (hit_proj);
			}
		}
	}
	out->distance = 0;
	return (-1);
}

bool	cone_intersect(t_cone *cone, const t_ray ray, t_intersection *out)
{
	t_calc		cc;
	float		t[2];
	t_vec3		hit_point;
	float		hit_proj;

	out->object = (t_object){.cone = cone, .type = CONE};
	out->normal = cone->axis;
	out->normal_calculated = true;
	if (!cone_calc(cone, ray, t, &cc))
		return (circle_intersect((t_circle){cone->bottom, cone->axis,
				cone->radius}, ray, &(out->distance), &(out->point)));
	hit_proj = cone_body_hit(cc, ray, out, t);
	if (hit_proj == -1)
		return (circle_intersect((t_circle){cone->bottom, cone->axis,
				cone->radius}, ray, &(out->distance), &(out->point)));
	if (circle_intersect((t_circle){cone->bottom, cone->axis, cone->radius}, 
		ray, &(t[1]), &(hit_point)) && out->distance > t[1])
	{
		out->distance = t[1];
		out->point = hit_point;
		return (true);
	}
	out->normal = calc_cone_normal(out->point, cone->top, cone->axis, hit_proj);
	out->normal_calculated = true;
	return (true);
}
