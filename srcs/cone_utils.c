/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmeubrin <lmeubrin@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:23:10 by lmeubrin          #+#    #+#             */
/*   Updated: 2025/04/10 11:23:50 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Calculate normal at hit point
t_vec3	calc_cone_normal(const t_vec3 hit_point, const t_vec3 top,
									const t_vec3 axis, const float m)
{
	t_vec3	axis_point;

	axis_point = vec3_add(top, vec3_multiply(axis, m));
	return (vec3_subtract(hit_point, axis_point));
}
