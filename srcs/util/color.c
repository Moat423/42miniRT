/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:28 by kwurster          #+#    #+#             */
/*   Updated: 2025/04/02 11:26:21 by lmeubrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

t_color	color_new(float r, float g, float b)
{
	return ((t_color){.x = r, .y = g, .z = b});
}

/// Convert Color to RGB
void	color_to_rgb(t_color color, uint8_t *r, uint8_t *g, uint8_t *b)
{
	*r = (uint8_t)(clamp(color.x, 0.0f, 1.0f) * 255.0f);
	*g = (uint8_t)(clamp(color.y, 0.0f, 1.0f) * 255.0f);
	*b = (uint8_t)(clamp(color.z, 0.0f, 1.0f) * 255.0f);
}

/// Create Color from RGB
t_color	color_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	t_color	result;

	result.x = (float)r / 255.0f;
	result.y = (float)g / 255.0f;
	result.z = (float)b / 255.0f;
	return (result);
}

t_color	vec3_to_color(t_vec3 vec)
{
	vec.x /= 255.0f;
	vec.y /= 255.0f;
	vec.z /= 255.0f;
	return (vec);
}

t_color	color_clamp(t_color color)
{
	t_color	result;

	result.x = clamp(color.x, 0.0f, 1.0f);
	result.y = clamp(color.y, 0.0f, 1.0f);
	result.z = clamp(color.z, 0.0f, 1.0f);
	return (result);
}
