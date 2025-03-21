/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:28:37 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/21 14:31:16 by kwurster         ###   ########.fr       */
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
