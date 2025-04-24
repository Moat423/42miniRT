/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 10:32:06 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"
#include <math.h>

static void	cylinder_body_quadr_coeff_calc(float abc[3], const t_ray ray,
											const t_cylinder *cyl)
{
	t_vec3	oc;
	float	d_dot_ax;
	float	oc_dot_ax;

	oc = vec3_subtract(ray.origin, cyl->bottom);
	d_dot_ax = vec3_dot(ray.direction, cyl->axis);
	oc_dot_ax = vec3_dot(oc, cyl->axis);
	abc[A] = 1 - d_dot_ax * d_dot_ax;
	abc[C] = vec3_squared_length(oc) - (oc_dot_ax * oc_dot_ax)
		- cyl->radius * cyl->radius;
	abc[B] = 2 * (vec3_dot(ray.direction, oc)
			- d_dot_ax * oc_dot_ax);
	return ;
}

static void	sort_t(float t[2])
{
	float	tmp;

	if (t[0] > t[1])
	{
		tmp = t[0];
		t[0] = t[1];
		t[1] = tmp;
	}
}

/// First picks the stable formula to calculate the root of quadratic equation
/// for us to avoid Catastrophic Cancellation
/// (when subtracting two numbers which are very close to each other)
/// Then to avoid calculating the unstable formula, it uses Vieta's formula
/// t1 * t2 = c / a
/// t2 = (c / a) / t1
/// TODO t[0] is not guaranteed to be smaller than t[1]
static int	cylinder_solve_quadratic_eq(float abc[3], float t[2])
{
	double	discr;

	if (fabsf(abc[A]) < EPSILON)
	{
		if (fabsf(abc[B]) < EPSILON)
			return (X_CIRCLE);
		else
		{
			t[0] = -abc[C] / abc[B];
			t[1] = t[0];
			return (X_TRUE);
		}
	}
	discr = (double)abc[B] *(double)abc[B] -4 *(double)abc[A] *(double)abc[C];
	if (fabs(discr) < EPSILON)
		return (X_CIRCLE);
	else if (discr < 0.0)
		return (X_FALSE);
	t[0] = (-abc[B] - copysignf(sqrtf((float)discr), abc[B])) / (2 * abc[A]);
	t[1] = abc[C] / (abc[A] * t[0]);
	return (X_TRUE);
}

bool	cylinder_intersect(t_cylinder *cylinder,
							const t_ray ray, t_intersection *out)
{
	float		t[2];
	float		abc[3];
	t_hit_type	hit;

	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	cylinder_body_quadr_coeff_calc(abc, ray, cylinder);
	hit = cylinder_solve_quadratic_eq(abc, t);
	if (hit == X_FALSE)
		return (false);
	else if (hit == X_CIRCLE)
		return (closer_circle_intersect(cylinder, ray, out));
	sort_t(t);
	if (cylinder_check_body_hit(cylinder, ray, t[0], out))
		return (true);
	return (cylinder_second_hit(cylinder, ray, t[1], out));
}
