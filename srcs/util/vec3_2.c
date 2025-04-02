/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:28:37 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/02 11:27:36 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

t_vec3	vec3_calc_length_and_normalize(t_vec3 v, float *length)
{
	*length = sqrtf(vec3_squared_length(v));
	if (*length == 0)
		return ((t_vec3){0});
	return (vec3_divide(v, *length));
}

t_vec3	vec3_pow(t_vec3 v, int scalar)
{
	t_vec3	result;

	result = vec3_new(1, 1, 1);
	while (scalar > 0)
	{
		result.x *= v.x;
		result.y *= v.y;
		result.z *= v.z;
		scalar--;
	}
	return (result);
}
