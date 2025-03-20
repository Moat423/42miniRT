/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:34 by kwurster          #+#    #+#             */
/*   Updated: 2025/03/19 14:20:31 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

t_vec3	vec3_new(float x, float y, float z)
{
	return ((t_vec3){x, y, z});
}

/// Dot product of two vectors
float	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float	vec3_squared_length(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

/// Cross product of two vectors
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	t_vec3	result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}

/// Normalize a vector
t_vec3	vec3_normalize(t_vec3 v)
{
	float	length;

	length = sqrtf(vec3_squared_length(v));
	v.x = v.x / length;
	v.y = v.y / length;
	v.z = v.z / length;
	return (v);
}
