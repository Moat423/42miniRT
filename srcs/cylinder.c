/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/31 13:25:31 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static	float	get_discriminant(float abc[3], const t_ray ray, const t_cylinder *cyl);

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

bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
{
	float	abc[3];
	float	discriminant;
	float	sqrt_d;
	float	t[4];
	t_vec3	hit_point;
	float	hit_proj;

	discriminant = get_discriminant(abc, ray, cylinder);
	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	out->normal = cylinder->axis;
	out->normal_calculated = true;
	if (discriminant < EPSILON && discriminant > -EPSILON)
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	if (discriminant < 0)
		return (false);
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	// cylinder body finite range lock
	if (interval_contains(ray.range, t[0]))
	{
		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[0]));
		hit_proj = projected_len_on_axis(cylinder, hit_point);
		if (hit_proj >= 0 && hit_proj <= cylinder->height)
		{
			out->distance = t[0];
			out->point = hit_point;
			out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
			set_intersect_normal(out, hit_proj);
			return (true);
		}
	}
	if (!interval_contains(ray.range, t[1]))
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[1]));
	hit_proj = projected_len_on_axis(cylinder, hit_point);
	if (hit_proj >= 0 && hit_proj <= cylinder->height)
	{
		// circle true hit
		if (closer_circle_intersect(cylinder, ray, &t[0], &(out->point)))
		{
			if (t[0] < t[1])
			{
				out->distance = t[0];
				return (true);
			}
		}
	}
	else
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	out->distance = t[1];
	out->point = hit_point;
	set_intersect_normal(out, hit_proj);
	return (true);
}

static	float	get_discriminant(float abc[3], const t_ray ray, const t_cylinder *cyl)
{
	t_vec3	oc;
	float	d_dot_ax;
	float	oc_dot_ax;

	oc = vec3_subtract(ray.origin, cyl->pos);
	d_dot_ax = vec3_dot(ray.direction, cyl->axis);
	oc_dot_ax = vec3_dot(oc, cyl->axis);
	abc[A] = vec3_squared_length(ray.direction) - d_dot_ax * d_dot_ax;
	abc[C] = vec3_squared_length(oc) - (oc_dot_ax * oc_dot_ax) - cyl->radius * cyl->radius;
	abc[B] = 2 * (vec3_dot(ray.direction, oc) 
			- d_dot_ax * oc_dot_ax);
	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
}
