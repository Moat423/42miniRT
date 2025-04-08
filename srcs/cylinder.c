/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/08 18:18:17 by lmeubrin         ###   ########.fr       */
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

// bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
// {
// 	float	abc[3];
// 	float	discriminant;
// 	float	sqrt_d;
// 	float	t[2];
// 	t_vec3	hit_point;
// 	float	hit_proj;
//
// 	discriminant = get_discriminant(abc, ray, cylinder);
// 	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
// 	out->normal = cylinder->axis;
// 	out->normal_calculated = true;
// 	if (discriminant < EPSILON && discriminant > -EPSILON)
// 		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
// 	if (discriminant < 0)
// 		return (false);
// 	sqrt_d = sqrtf(discriminant);
// 	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
// 	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
// 	// cylinder body finite range lock
// 	if (interval_contains(ray.range, t[0]))
// 	{
// 		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[0]));
// 		hit_proj = projected_len_on_axis(cylinder, hit_point);
// 		if (hit_proj >= 0 && hit_proj <= cylinder->height)
// 		{
// 			out->distance = t[0];
// 			out->point = hit_point;
// 			set_intersect_normal(out, hit_proj);
// 			return (true);
// 		}
// 	}
// 	if (!interval_contains(ray.range, t[1]))
// 		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
// 	hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[1]));
// 	hit_proj = projected_len_on_axis(cylinder, hit_point);
// 	if (hit_proj >= 0 && hit_proj <= cylinder->height)
// 	{
// 		// circle true hit
// 		if (closer_circle_intersect(cylinder, ray, &t[0], &(out->point)))
// 		{
// 			if (t[0] < t[1])
// 			{
// 				out->distance = t[0];
// 				return (true);
// 			}
// 		}
// 	}
// 	else
// 		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
// 	out->distance = t[1];
// 	out->point = hit_point;
// 	set_intersect_normal(out, hit_proj);
// 	return (true);
// }

// static t_calc	prep_cyl_calc(const t_ray ray, const t_cylinder *cyl)
// {
// 	t_calc	cc;
//
// 	cc.oc = vec3_subtract(ray.origin, cyl->top);
// 	cc.d_dot_n = vec3_dot(ray.direction, cyl->axis);
// 	cc.oc_dot_n = vec3_dot(cc.oc, cyl->axis);
// 	cc.coeff = cyl->radius * cyl->radius;
// 	cc.height = cyl->height;
// 	return (cc);
// }
//
// static float	get_discriminant(const t_vec3 ray_dir, const t_calc cc,
// 							float abc[3])
// {
// 	abc[A] = vec3_squared_length(ray_dir) - cc.d_dot_n * cc.d_dot_n;
// 	abc[C] = vec3_squared_length(cc.oc) - (cc.oc_dot_n * cc.oc_dot_n) - cc.coeff;
// 	abc[B] = 2 * (vec3_dot(ray_dir, cc.oc) 
// 			- cc.d_dot_n * cc.oc_dot_n);
// 	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
// }

// static int	solve_quadratic_eq_cylinder(double coef[3], double t[2])
// {
// 	double	discriminant;
// 	double	q;
//
// 	discriminant = coef[B] * coef[B] - 4 * coef[A] * coef[C];
// 	if (discriminant < 0)
// 		return (0);
// 	discriminant = sqrtf(discriminant);
// 	if (coef[B] < 0)
// 		q = -0.5 * (coef[B] - discriminant);
// 	else
// 		q = -0.5 * (coef[B] + discriminant);
// 	t[0] = q / coef[A];
// 	if (q != 0)
// 		t[B] = coef[C] / q;
// 	else
// 		t[B] = INFINITY;
// 	return (1);
// }

static int	solve_quadratic_eq_cylinder(float abc[3], float t[2])
{
	float		discriminant;
	float		sqrt_d;
	float		q;

	discriminant = (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
	if (discriminant < EPSILON && discriminant > -EPSILON)
		return (CIRCLE);
	if (discriminant < 0)
		return (FALSE);
	if (fabs(abc[C]) < EPSILON) // ray origin on surface
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
	if (abc[1] < 0)
		q = -0.5 * (abc[1] - discriminant);
	else
		q = -0.5 * (abc[1] + discriminant);
	t[0] = q / abc[0];
	if (fabs(q) > EPSILON)
		t[1] = abc[2] / q;
	else
		t[1] = INFINITY;
	return (1);
	// if (fabs(abc[C]) < EPSILON)
	// 	return (false);
	// if (fabs(abc[A]) < EPSILON)
	// {
	// 	if (fabs(abc[B]) < EPSILON)
	// 		return (false);
	// 	t[0] = -abc[C] / abc[B];
	// 	t[1] = t[0];
	// 	return (true);
	// }
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	return (true);
}

// static int	cylinder_calc(const t_cylinder *cyl, const t_ray ray,
// 						float t[2])
// {
// 	float		abc[3];
// 	float		discriminant;
// 	float		sqrt_d;
//
// 	cylinder_body_quadr_coeff_calc(abc, ray, cyl);
// 	solve_quadratic_eq_cylinder(abc, t);
// 	discriminant = 
// 	if (discriminant < EPSILON && discriminant > -EPSILON)
// 		return (CIRCLE);
// 	if (discriminant < 0)
// 		return (FALSE);
// 	// if (fabs(abc[C]) < EPSILON)
// 	// 	return (false);
// 	// if (fabs(abc[A]) < EPSILON)
// 	// {
// 	// 	if (fabs(abc[B]) < EPSILON)
// 	// 		return (false);
// 	// 	t[0] = -abc[C] / abc[B];
// 	// 	t[1] = t[0];
// 	// 	return (true);
// 	// }
// 	sqrt_d = sqrtf(discriminant);
// 	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
// 	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
// 	return (true);
// }

bool	cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersection *out)
{
	// float	abc[3];
	// float	discriminant;
	// float	sqrt_d;
	float	t[2];
	t_vec3	hit_point;
	float	hit_proj;
	float	abc[3];
	int		is_circle_hit;

	// discriminant = get_discriminant(abc, ray, cylinder);
	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	out->normal = cylinder->axis;
	out->normal_calculated = true;
	cylinder_body_quadr_coeff_calc(abc, ray, cylinder);
	is_circle_hit = solve_quadratic_eq_cylinder(abc, t);
	if (is_circle_hit == FALSE)
		return (false);
	else if (is_circle_hit == CIRCLE)
	{
		out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
		out->normal = cylinder->axis;
		out->normal_calculated = true;
		return (closer_circle_intersect(cylinder, ray, &(out->distance), &(out->point)));
	}
	// cylinder body finite range lock
	if (interval_contains(ray.range, t[0]))
	{
		hit_point = vec3_add(ray.origin, vec3_multiply(ray.direction, t[0]));
		hit_proj = projected_len_on_axis(cylinder, hit_point);
		if (hit_proj >= 0 && hit_proj <= cylinder->height)
		{
			out->distance = t[0];
			out->point = hit_point;
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

