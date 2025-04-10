/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:28:56 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 10:30:10 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

/// @brief Calculate the unnormalized surface normal of a cylinder at a given
/// intersection point
/// @param intersection The intersection point
/// @param m The projection of the intersection point on the cylinder axis
static void	set_intersect_normal(t_intersection *intersection, float m)
{
	intersection->normal = vec3_subtract(
			intersection->point,
			vec3_add(intersection->object.cylinder->bottom,
				vec3_multiply(intersection->object.cylinder->axis, m))
			);
	intersection->normal_calculated = false;
}

/// returns the projection of vector v on the axis of the cylinder
/// also known as "m"
/// if the resulting float value is between 0 and height, 
/// the point is on the cylinder body
static float	projected_len_on_axis(t_cylinder *cyl, t_vec3 v)
{
	return (vec3_dot(cyl->axis, vec3_subtract(v, cyl->bottom)));
}

bool	cylinder_check_body_hit(t_cylinder *cylinder, const t_ray ray,
							const float t, t_intersection *out)
{
	t_vec3	hit_point;
	float	hit_proj;

	if (interval_contains(ray.range, t))
	{
		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
		hit_proj = projected_len_on_axis(cylinder, hit_point);
		if (hit_proj >= 0 && hit_proj <= cylinder->height)
		{
			out->distance = t;
			out->point = hit_point;
			set_intersect_normal(out, hit_proj);
			return (true);
		}
	}
	return (false);
}

// if the first hit was not a true hit on the cylinder body 
// or the ray was not aligned with the axis
// then this functions is used to check wich next closest hit is viable
bool	cylinder_second_hit(t_cylinder *cylinder, const t_ray ray,
							const float t, t_intersection *out)
{
	t_vec3	hit_point;
	float	hit_proj;

	if (!interval_contains(ray.range, t))
		return (closer_circle_intersect(cylinder, ray, out));
	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
	hit_proj = projected_len_on_axis(cylinder, hit_point);
	if (hit_proj >= 0 && hit_proj <= cylinder->height)
	{
		if (closer_circle_intersect(cylinder, ray, out))
			if (out->distance < t)
				return (true);
	}
	else
		return (closer_circle_intersect(cylinder, ray, out));
	out->distance = t;
	out->point = hit_point;
	set_intersect_normal(out, hit_proj);
	return (true);
}

