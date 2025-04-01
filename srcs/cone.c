/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:28:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/01 15:10:36 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// gets discriminant of cone
// the axis is normalized and therefore the normal of the cone
// coeff: 1 + slant * slant
// a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
// b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
// c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
// static	float	get_discriminant(float abc[3], const t_ray ray, 
// 								const t_cone *cone)
// {
// 	t_vec3	oc;
// 	float	d_dot_ax;
// 	float	oc_dot_ax;
// 	float	coeff;
//
// 	oc = vec3_subtract(ray.origin, cone->top);
// 	d_dot_ax = vec3_dot(ray.direction, cone->axis);
// 	oc_dot_ax = vec3_dot(oc, cone->axis);
// 	coeff = 1 + cone->slant * cone->slant;
// 	abc[A] = vec3_squared_length(ray.direction) - coeff * d_dot_ax * d_dot_ax;
// 	abc[C] = vec3_squared_length(oc) - coeff * oc_dot_ax * oc_dot_ax;
// 	abc[B] = 2 * (vec3_dot(ray.direction, oc) - coeff * d_dot_ax * oc_dot_ax);
// 	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
// }

// Calculate normal at hit point
// static t_vec3	calc_normal(const t_ray ray, const t_cone *cone, 
// 							float t, t_cone_calc cc)
// {
// 	t_vec3	hit_point;
// 	t_vec3	axis_point;
//
// 	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t));
// 	axis_point = vec3_add(cone->top, vec3_multiply(cone->axis, cc.m));
// 	return (vec3_normalize(vec3_subtract(hit_point, axis_point)));
// }

// gets discriminant of cone
// the axis is normalized and therefore the normal of the cone
// coeff: 1 + slant * slant
// a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
// b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
// c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
static	float	get_discriminant(const t_vec3 ray_dir, const t_cone_calc cc,
							float abc[3])
{
	abc[A] = vec3_squared_length(ray_dir) - cc.coeff * cc.d_dot_n * cc.d_dot_n;
	abc[C] = vec3_squared_length(cc.oc) - cc.coeff * cc.oc_dot_n * cc.oc_dot_n;
	abc[B] = 2 * (vec3_dot(ray_dir, cc.oc) 
			- cc.coeff * cc.d_dot_n * cc.oc_dot_n);
	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
}

static t_cone_calc	prep_cone_calc(const t_ray ray, const t_cone *cone)
{
	t_cone_calc	cc;

	cc.oc = vec3_subtract(ray.origin, cone->top);
	cc.d_dot_n = vec3_dot(ray.direction, cone->axis);
	cc.oc_dot_n = vec3_dot(cc.oc, cone->axis);
	cc.coeff = 1 + cone->slant * cone->slant;
	return (cc);
}

static bool	cone_calc(const t_cone *cone, const t_ray ray, float t[2], t_cone_calc *cc)
{
	float		abc[3];
	float		discriminant;
	float		sqrt_d;

	*cc = prep_cone_calc(ray, cone);
	discriminant = get_discriminant(ray.direction, *cc, abc);
	if (discriminant < EPSILON)
		return (false);
	if (fabs(abc[A]) < EPSILON)
	{
		// Special case - solve linear equation
		if (fabs(abc[B]) < EPSILON)
			return (false);  // No solution
		t[0] = -abc[C] / abc[B];
		t[1] = t[0]; // Both solutions are the same
		return (true);
	}
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	return (true);
}

bool	cone_intersect(t_cone *cone, t_ray ray, t_intersection *out)
{
	t_cone_calc	cc;
	float		t[3];
	int			i;
	t_vec3		hit_point;
	float		closest_t;
	bool		hit_found;

	hit_found = false;
	closest_t = INFINITY;
	if (!cone_calc(cone, ray, t, &cc))
		return (false);
	out->object = (t_object){.cone = cone, .type = CONE};
	out->normal = cone->axis;
	out->normal_calculated = true;
	i = -1;
	while (++i < 2)
	{
		if (interval_contains(ray.range, t[i]))
		{
			// Calculate hit point
			hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[i]));
			// Calculate m = (D·V)*t + (X·V)
			cc.m = cc.d_dot_n * t[i] + cc.oc_dot_n;
			// Check if hit point is within the height bounds
			if (cc.m >= 0 && cc.m <= cone->height)
			{
				// This is a valid hit
				if (t[i] < closest_t)
				{
					closest_t = t[i];
					out->distance = t[i];
					out->point = hit_point;
					// Calculate the normal at the hit point
					t_vec3 hit_to_axis = vec3_add(cone->top, vec3_multiply(cone->axis, cc.m));
					out->normal = vec3_normalize(vec3_subtract(hit_point, hit_to_axis));
					out->normal_calculated = true;
					hit_found = true;
				}
			}
		}
	}
	// Check bottom cap (circle) intersection
	bool cap_hit = circle_intersect(
			(t_circle){cone->bottom, cone->axis, cone->radius}, 
			ray, 
			&t[2], 
			&hit_point
			);
	if (cap_hit && interval_contains(ray.range, t[2]) && t[2] < closest_t)
	{
		closest_t = t[2];
		out->distance = t[2];
		out->point = hit_point;
		out->normal = vec3_multiply(cone->axis, -1);  // Normal points outward from the cap
		out->normal_calculated = true;
		hit_found = true;
	}
	return hit_found;
}

// bool	old_cone_intersect(t_cone *cone, t_ray ray, t_intersection *out)
// {
// 	float	discriminant;
// 	float	abc[3];
// 	float	sqrt_d;
// 	float	t[3];
// 	int		i;
// 	t_vec3	hit_point;
//
// 	i = -1;
// 	discriminant = get_discriminant(abc, ray, cone);
// 	if (discriminant < 0)
// 		return (false);
// 	out->object = (t_object){.cone = cone, .type = CONE};
// 	out->normal = cone->axis;
// 	out->normal_calculated = true;
// 	sqrt_d = sqrtf(discriminant);
// 	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
// 	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
// 	circle_intersect((t_circle){cone->bottom, cone->axis, cone->radius}, ray, &t[2], &(out->point));
// 	// if (t[2] <= t[0] && t[2] <= t[1])
// 	while (++i < 2)
// 	{
// 		if (interval_contains(ray.range, t[i]))
// 		{
//
// 		}
// 	}
// }
