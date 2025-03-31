/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:28:02 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/03/31 16:20:50 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	ft_insertion_sort(float *arr, size_t size)
{
	int		i;
	int		j;
	float	key;

	i = 1;
	while (i < size)
	{
		key = arr[i];
		j = i - 1;
		while (j >= 0 && arr[j] >= key)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

// gets discriminant of cone
// the axis is normalized and therefore the normal of the cone
// coeff: 1 + slant * slant
// a = (d dot d) - (1 + slant * slant) * (d dot axis)^2
// b = root((d dot oc) - (1 + slant * slant) * (d dot axis) (oc dot axis)
// c = (oc dot oc) - (1 + slant * slant) * (oc dot axis)^2
static	float	get_discriminant(float abc[3], const t_ray ray, 
								const t_cone *cone)
{
	t_vec3	oc;
	float	d_dot_ax;
	float	oc_dot_ax;
	float	coeff;

	oc = vec3_subtract(ray.origin, cone->top);
	d_dot_ax = vec3_dot(ray.direction, cone->axis);
	oc_dot_ax = vec3_dot(oc, cone->axis);
	coeff = 1 + cone->slant * cone->slant;
	abc[A] = vec3_squared_length(ray.direction) - coeff * d_dot_ax * d_dot_ax;
	abc[C] = vec3_squared_length(oc) - coeff * oc_dot_ax * oc_dot_ax;
	abc[B] = 2 * (vec3_dot(ray.direction, oc) - coeff * d_dot_ax * oc_dot_ax);
	return (abc[B] * abc[B] - 4 * abc[A] * abc[C]);
}

bool	cone_intersect(t_cone *cone, t_ray ray, t_intersection *out)
{
	float	discriminant;
	float	abc[3];
	float	sqrt_d;
	float	t[3];
	int		i;
	t_vec3	hit_point;

	i = -1;
	discriminant = get_discriminant(abc, ray, cone);
	// if (discriminant < 0)
	// 	return (true) // is elipse;
	out->object = (t_object){.cone = cone, .type = CONE};
	out->normal = cone->axis;
	out->normal_calculated = true;
	sqrt_d = sqrtf(discriminant);
	t[0] = (-abc[B] - sqrt_d) / (2 * abc[A]);
	t[1] = (-abc[B] + sqrt_d) / (2 * abc[A]);
	circle_intersect((t_circle){cone->bottom, cone->axis, cone->radius}, ray, &t[2], &(out->point));
	// if (t[2] <= t[0] && t[2] <= t[1])
	while (++i < 2)
	{
		if (interval_contains(ray.range, t[i]))
		{
			
		}
	}
}
