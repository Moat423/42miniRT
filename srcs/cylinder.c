/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/25 12:58:07 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

bool	circle_intersect(t_cylinder *cylinder, t_ray, t_intersection *out);

bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
{
	t_vec3	oc;
	float	abc[3];
	float	discriminant;
	float	sqrt_d;
	float	t[2];
	t_vec3	hit_point;
	float	hit_proj; //projection of hit_point along axis
	int		i;

	i = -1;
	oc = vec3_subtract(ray.origin, cylinder->pos);
	abc[A] = vec3_squared_length(ray.direction) - vec3_dot(ray.direction, cylinder->axis) * vec3_dot(ray.direction, cylinder->axis);
	abc[C] = vec3_squared_length(oc) - (vec3_dot(oc, cylinder->axis) * vec3_dot(oc, cylinder->axis)) - cylinder->radius * cylinder->radius;
	abc[B] = 2 * vec3_dot(ray.direction, oc) - vec3_dot(ray.direction, cylinder->axis) * vec3_dot(oc, cylinder->axis);
	discriminant = abc[B] * abc[B] - 4 * abc[A] * abc[C];
	if (discriminant < EPSILON || discriminant > -EPSILON)
	{
		circle_intersect(cylinder, ray, out);
	}
	if (discriminant < 0)
		return (false);
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	while (++i < 2)
	{
		if (t[i] < ray.range.min || !interval_contains(ray.range, t[i]))
			continue ;
		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, out->distance));
		hit_proj = vec3_dot(vec3_subtract(hit_point, cylinder->top), cylinder->axis);
		if (hit_proj <= 0 && hit_proj >= -cylinder->height)
		{
			out->distance = t[i];
			out->point = hit_point;
			out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
			return (true);
		}
	}
	return (false);
}

bool	circle_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
{
	
}
