/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_shadow.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:29:51 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 12:24:57 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

bool	cylinder_body_hit_anywhere(t_cylinder *cylinder, const t_ray ray,
							const float t)
{
	t_vec3	hit_point;
	float	hit_proj;

	if (interval_contains(ray.range, t))
	{
		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
		hit_proj = (vec3_dot(cylinder->axis,
					vec3_subtract(hit_point, cylinder->bottom)));
		if (hit_proj >= 0 && hit_proj <= cylinder->height)
			return (true);
	}
	return (false);
}


// check if cylinder cant  be hit and return cirlce hit,
// but circle hit is harder to calc,
// therefore if body is hit, return there, else calc the cicle hits
bool	cylinder_hit_anywhere(t_cylinder *cylinder, const t_ray ray)
{
	float	t[2];
	float	abc[3];
	int		is_circle_hit;

	cylinder_body_quadr_coeff_calc(abc, ray, cylinder);
	is_circle_hit = cylinder_solve_quadratic_eq(abc, t);
	if (is_circle_hit == FALSE)
		return (false);
	else if (is_circle_hit == CIRCLE)
	{
		if (circle_hit_anywhere((t_circle){cylinder->top, cylinder->axis,
				cylinder->radius}, ray))
			return (true);
		return (circle_hit_anywhere((t_circle){cylinder->bottom, cylinder->axis,
				cylinder->radius}, ray));
	}
	if (cylinder_body_hit_anywhere(cylinder, ray, t[0]))
		return (true);
	if (cylinder_body_hit_anywhere(cylinder, ray, t[1]))
		return (true);
	if (circle_hit_anywhere((t_circle){cylinder->top, cylinder->axis,
			cylinder->radius}, ray))
		return (true);
	return (circle_hit_anywhere((t_circle){cylinder->bottom, cylinder->axis,
			cylinder->radius}, ray));
}
