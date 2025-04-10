/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 10:12:25 by lmeubrin         ###   ########.fr       */
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

static void	cylinder_body_quadr_coeff_calc(float abc[3], const t_ray ray, const t_cylinder *cyl)
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
	return ;
}

static int	solve_quadratic_eq_cylinder(float abc[3], float t[2])
{
	float		discriminant;

	discriminant = (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
	if (discriminant < EPSILON && discriminant > -EPSILON)
		return (CIRCLE);
	if (discriminant < 0 || (fabs(abc[C]) < EPSILON))
		return (FALSE);
	if (fabs(abc[A]) < EPSILON)
	{
		if (fabs(abc[B]) < EPSILON)
			return (FALSE);
		t[0] = -abc[C] / abc[B];
		t[1] = t[0];
		return (TRUE);
	}
	discriminant = sqrtf(discriminant);
	t[0] = (-abc[B] - discriminant) / (2 * abc[A]);
	t[1] = (-abc[B] + discriminant) / (2 * abc[A]);
	return (TRUE);
}

bool	check_body_hit(t_cylinder *cylinder, const t_ray ray, const float t,
						t_intersection *out)
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
							t_intersection *out, const float t)
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

bool	cylinder_intersect(t_cylinder *cylinder,
							const t_ray ray, t_intersection *out)
{
	float	t[2];
	t_vec3	hit_point;
	float	hit_proj;
	float	abc[3];
	int		is_circle_hit;

	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	cylinder_body_quadr_coeff_calc(abc, ray, cylinder);
	is_circle_hit = solve_quadratic_eq_cylinder(abc, t);
	if (is_circle_hit == FALSE)
		return (false);
	else if (is_circle_hit == CIRCLE)
		return (closer_circle_intersect(cylinder, ray, out));
	if (check_body_hit(cylinder, ray, t[0], out))
		return (true);
	if (!interval_contains(ray.range, t[1]))
		return (closer_circle_intersect(cylinder, ray, out));
	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[1]));
	hit_proj = projected_len_on_axis(cylinder, hit_point);
	if (hit_proj >= 0 && hit_proj <= cylinder->height)
	{
		if (closer_circle_intersect(cylinder, ray, out))
			if (out->distance < t[1])
				return (true);
	}
	else
		return (closer_circle_intersect(cylinder, ray, out));
	out->distance = t[1];
	out->point = hit_point;
	set_intersect_normal(out, hit_proj);
	return (true);
}
