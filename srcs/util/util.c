/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:55:31 by kwurster          #+#    #+#             */
/*   Updated: 2025/05/22 13:03:19 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// Helper function to clamp a float value between min and max
float	clamp(float value, float min, float max)
{
	return (fminf(fmaxf(value, min), max));
}

float	image_aspect_ratio(t_scene *scene)
{
	return ((float)scene->image_width / (float)scene->image_height);
}

bool	equal(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

// binary exponentiation
float	powi(float x, int n)
{
	float	result;

	result = 1;
	if (n < 0)
	{
		x = 1.0f / x;
		n = -n;
	}
	while (n > 0)
	{
		if (n & 1)
		{
			result *= x;
		}
		x *= x;
		n >>= 1;
	}
	return (result);
}

/// Convert Color to RGB
void	color_to_rgb2(t_color color, uint8_t *rgba)
{
	rgba[0] = (uint8_t)(clamp(color.x, 0.0f, 1.0f) * 255.0f);
	rgba[1] = (uint8_t)(clamp(color.y, 0.0f, 1.0f) * 255.0f);
	rgba[2] = (uint8_t)(clamp(color.z, 0.0f, 1.0f) * 255.0f);
	rgba[3] = 255;
}
