/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:57:20 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/09 13:23:48 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

bool	circle_intersect(t_circle circle, t_ray ray, 
						float *o_dist, t_vec3 *o_pt)
{
	float	t;
	float	denom;
	t_vec3	hit_point;

	denom = vec3_dot(circle.normal, ray.direction);
	if (denom < EPSILON && denom > -EPSILON)
		return (false);
	t = vec3_dot(vec3_subtract(circle.center, ray.origin), circle.normal)
		/ denom;
	if (!interval_contains(ray.range, t))
		return (false);
	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
	// the (1.0f + EPSILON) is to account for floating point errors, can be taken out
	if (vec3_squared_length(vec3_subtract(hit_point, circle.center))
		<= circle.radius * circle.radius * (1.0f + EPSILON))
	{
		*o_dist = t;
		*o_pt = hit_point;
		return (true);
	}
	return (false);
}

bool	closer_circle_intersect(t_cylinder *cylinder, t_ray ray, 
								t_intersection *out)
{
	t_vec3	hit_point[2];
	float	hit_dist[2];

	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	out->normal = cylinder->axis;
	out->normal_calculated = true;
	if (!circle_intersect((t_circle){cylinder->top, cylinder->axis, cylinder->radius}, ray, &(hit_dist[0]), &(hit_point[0])))
		return (circle_intersect((t_circle){cylinder->bottom, cylinder->axis, cylinder->radius}, ray, &(out->distance), &(out->point)));
	if (!circle_intersect((t_circle){cylinder->bottom, cylinder->axis, cylinder->radius}, ray, &(hit_dist[1]), &(hit_point[1])))
	{
		out->distance = hit_dist[0];
		out->point = hit_point[0];
		return (true);
	}
	if (hit_dist[0] < hit_dist[1])
	{
		out->distance = hit_dist[0];
		out->point = hit_point[0];
		return (true);
	}
	out->distance = hit_dist[1];
	out->point = hit_point[1];
	return (true);
}
