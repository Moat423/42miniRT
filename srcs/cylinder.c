/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:38:32 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 10:30:18 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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

static int	cylinder_solve_quadratic_eq(float abc[3], float t[2])
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

bool	cylinder_intersect(t_cylinder *cylinder,
							const t_ray ray, t_intersection *out)
{
	float	t[2];
	float	abc[3];
	int		is_circle_hit;

	out->object = (t_object){.cylinder = cylinder, .type = CYLINDER};
	cylinder_body_quadr_coeff_calc(abc, ray, cylinder);
	is_circle_hit = cylinder_solve_quadratic_eq(abc, t);
	if (is_circle_hit == FALSE)
		return (false);
	else if (is_circle_hit == CIRCLE)
		return (closer_circle_intersect(cylinder, ray, out));
	if (cylinder_check_body_hit(cylinder, ray, t[0], out))
		return (true);
	return (cylinder_second_hit(cylinder, ray, t[1], out));
}
