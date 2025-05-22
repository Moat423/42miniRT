/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:23:10 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/05/22 15:39:24 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	calc_cone_normal(const t_vec3 hit_point, const t_cone *cone,
						const float hit_proj)
{
	t_vec3	axis_point;
	t_vec3	axis_to_point;
	t_vec3	normal;

	axis_point = vec3_add(cone->top, vec3_multiply(cone->axis, hit_proj));
	axis_to_point = vec3_subtract(hit_point, axis_point);
	normal = vec3_subtract(axis_to_point, 
			vec3_multiply(cone->axis,
				cone->slant * cone->slant * hit_proj));
	return (normal);
}
