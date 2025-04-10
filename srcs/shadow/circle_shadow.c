/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle_shadow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:45:13 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 11:46:34 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

bool	circle_hit_anywhere(t_circle circle, const t_ray ray)
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
	if (vec3_squared_length(vec3_subtract(hit_point, circle.center))
		<= circle.radius * circle.radius)
		return (true);
	return (false);
}
